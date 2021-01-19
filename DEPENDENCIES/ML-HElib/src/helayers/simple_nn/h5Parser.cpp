/*
* MIT License
*
* Copyright (c) 2020 International Business Machines
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include <queue>
#include <vector>
#include <fstream>

#include "h5Parser.h"

using namespace H5;
using namespace std;
using namespace boost::numeric::ublas;

namespace helayers {

bool H5Parser::objectExists(const string& name) const
{
  try {
    H5G_stat_t dummy;
    H5::Exception::dontPrint();
    file.getObjinfo(name, dummy);
    return true;
  } catch (H5::Exception&) {
    return false;
  }
}

H5Parser::H5Parser(string f) : file_name(f)
{
  // test file exists, for a nicer error message
  ifstream test(f);
  if (!test.good())
    throw invalid_argument("File " + f + " doesn't exist or unreadable");

  file = H5File(f, H5F_ACC_RDWR);
}

H5Parser::~H5Parser() { file.close(); }

void H5Parser::readData(const std::string& path, double& val) const
{
  // TODO: move these to a seperate function
  DataSet dataset = file.openDataSet(path);
  DataType datatype = dataset.getDataType();
  DataSpace dataspace = dataset.getSpace();
  H5T_class_t classt = datatype.getClass();

  if (classt != H5T_INTEGER && classt != H5T_FLOAT)
    throw DataTypeIException("Parser::readData");

  int rank = dataspace.getSimpleExtentNdims();

  if (rank != 1) {
    throw invalid_argument(
        "A scalar value was expected, while the received value has rank " +
        to_string(rank) + ".");
  }

  hsize_t dim;
  dataspace.getSimpleExtentDims(&dim);

  if (dim != 1) {
    throw invalid_argument(
        "A scalar value was expected, while the received value has " +
        to_string(dim) + " elements.");
  }

  if (classt == H5T_INTEGER) {
    int tmpVal;
    dataset.read(&tmpVal, PredType::NATIVE_INT);
    val = (double)tmpVal;
  } else {
    dataset.read(&val, PredType::NATIVE_DOUBLE);
  }
}

void H5Parser::readData(const std::string& path, tensor<double>& vals) const
{
  DataSet dataset = file.openDataSet(path);
  DataType datatype = dataset.getDataType();
  DataSpace dataspace = dataset.getSpace();
  H5T_class_t classt = datatype.getClass();

  if (classt != H5T_INTEGER && classt != H5T_FLOAT)
    throw DataTypeIException("Parser::readData");

  int rank = dataspace.getSimpleExtentNdims();
  std::vector<hsize_t> dims(rank);
  dataspace.getSimpleExtentDims(dims.data());

  std::vector<size_t> dimsCast(dims.size());

  for (size_t i = 0; i < dims.size(); i++) {
    dimsCast[i] = (size_t)dims[i];
  }

  basic_extents<size_t> shape(dimsCast);
  vals.reshape(shape);

  if (classt == H5T_INTEGER) {
    tensor<int> tmpVals(shape);
    dataset.read(tmpVals.data(), PredType::NATIVE_INT);
    for (size_t i = 0; i < tmpVals.size(); i++) {
      vals[i] = (double)tmpVals[i];
    }
  } else {
    dataset.read(vals.data(), PredType::NATIVE_DOUBLE);
  }
}

void H5Parser::readData(const std::string& path,
                        std::vector<double>& vals,
                        std::vector<int>& dims) const
{
  DataSet dataset = file.openDataSet(path);
  DataType datatype = dataset.getDataType();
  DataSpace dataspace = dataset.getSpace();
  H5T_class_t classt = datatype.getClass();

  if (classt != H5T_INTEGER && classt != H5T_FLOAT)
    throw DataTypeIException("Parser::parseBias");

  int rank = dataspace.getSimpleExtentNdims();
  hsize_t* dd = new hsize_t[rank];
  dataspace.getSimpleExtentDims(dd);
  dims.clear();
  for (int i = 0; i < rank; ++i)
    dims.push_back(dd[i]);
  int elmnts = dataspace.getSimpleExtentNpoints();
  if (classt == H5T_INTEGER) {
    std::vector<int> tmpVals;
    tmpVals.resize(elmnts);
    dataset.read(tmpVals.data(), PredType::NATIVE_INT);
    for (auto const& val : tmpVals)
      vals.push_back(val);
  } else {
    vals.resize(elmnts);
    dataset.read(vals.data(), PredType::NATIVE_DOUBLE);
  }
  delete[] dd;
}

std::vector<double> H5Parser::parseBias(string path) const
{
  string var = path + "/bias:0";

  if (!objectExists(var)) {
    return std::vector<double>(0);
  }

  DataSet dataset = file.openDataSet(var);
  DataType datatype = dataset.getDataType();
  DataSpace dataspace = dataset.getSpace();
  H5T_class_t classt = datatype.getClass();

  if (classt != 1)
    throw DataTypeIException("Parser::parseBias");

  int rank = dataspace.getSimpleExtentNdims();
  if (rank != BIAS_RANK) {
    string msg = "Data space with rank = ";
    msg += DENSE_RANK + " was expected. Instead data space with rank = ";
    msg += rank + " was received.";
    throw DataSpaceIException(msg);
  }

  int elmnts = dataspace.getSimpleExtentNpoints();

  double* data = new double[elmnts];

  dataset.read(data, PredType::NATIVE_DOUBLE);

  std::vector<double> out;
  for (int i = 0; i < elmnts; i++)
    out.push_back(data[i]);

  delete[] data;
  dataspace.close();
  datatype.close();
  dataset.close();

  return out;
}

std::vector<std::vector<double>> H5Parser::parseFC(string path) const
{
  string var = path + "/kernel:0";

  if (!objectExists(var)) {
    cout << "Warning: " << var << "not found in h5file" << endl;
    return std::vector<std::vector<double>>(0);
  }

  DataSet dataset = file.openDataSet(var);
  DataType datatype = dataset.getDataType();
  DataSpace dataspace = dataset.getSpace();
  int rank = dataspace.getSimpleExtentNdims();
  if (rank != DENSE_RANK) {
    string msg = "Data space with rank = ";
    msg += DENSE_RANK + " was expected. Instead data space with rank = ";
    msg += rank + " was received.";
    throw DataSpaceIException(msg);
  }

  hsize_t dims[DENSE_RANK];
  dataspace.getSimpleExtentDims(dims);
  size_t rows = dims[0];
  size_t cols = dims[1];

  // For compatibility with Keras h5 output files, we dump and parse the weights
  // as floats rather
  // than doubles
  float* data = new float[rows * cols];
  float** md = new float*[rows];
  for (size_t i = 0; i < rows; ++i)
    md[i] = data + i * cols;

  dataset.read(data, PredType::NATIVE_FLOAT);

  std::vector<std::vector<double>> v(rows, std::vector<double>(cols, 0));
  // Assign 2D vector
  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      v[i][j] = md[i][j];

  delete[] md;
  delete[] data;
  dataspace.close();
  datatype.close();
  dataset.close();

  return v;
}

std::vector<std::vector<std::vector<double>>> H5Parser::parseFilters(
    string path) const
{
  string var = path + "/kernel:0";

  if (!objectExists(var)) {
    throw invalid_argument("Missing var " + var);
    // return std::vector<std::vector<std::vector<double> > >(0);
  }

  DataSet dataset = file.openDataSet(var);
  DataType datatype = dataset.getDataType();
  DataSpace dataspace = dataset.getSpace();
  int rank = dataspace.getSimpleExtentNdims();
  if (rank != FILTER_RANK) {
    string msg = "Data space with rank = ";
    msg += FILTER_RANK + " was expected. Instead data space with rank = ";
    msg += rank + " was received.";
    throw DataSpaceIException(msg);
  }

  hsize_t dims[FILTER_RANK];
  dataspace.getSimpleExtentDims(dims);
  size_t rows = dims[0];
  size_t cols = dims[1];
  size_t filter_dim = dims[2]; // should be 1 always
  size_t filters_num = dims[3];

  //  cout << "rows=" << rows << endl;
  //  cout << "cols=" << cols << endl;
  //  cout << "dim=" << filter_dim << endl;
  //  cout << "num=" << filters_num << endl;

  // For compatibility with Keras h5 output files, we dump and parse the weights
  // as floats rather
  // than doubles
  float* data = new float[rows * cols * filter_dim * filters_num];

  dataset.read(data, PredType::NATIVE_FLOAT);

  std::vector<std::vector<std::vector<double>>> filters(
      filters_num,
      std::vector<std::vector<double>>(rows, std::vector<double>(cols, 0)));
  // Assign 3D vector
  //  cout << "data[0]=" << data[0] << endl;
  int indx = 0;
  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      for (size_t d = 0; d < filters_num; d++) {
        //        cout << "d=" << data[indx++] << endl;
        filters[d][i][j] = data[indx++];
      }

  delete[] data;
  dataspace.close();
  datatype.close();
  dataset.close();

  return filters;
}

/*bool H5Parser::sameHierarchy(const H5Parser& other) const{
 priority_queue<string> thisHierarchy, otherHierarchy;

 thisHierarchy.push("/");
 otherHierarchy.push("/");

 while (!thisHierarchy.empty())
 {
 if (otherHierarchy.empty())
 {
 return false;
 }

 string thisCurrent = thisHierarchy.pop();
 string otherCurrent = otherHierarchy.pop();

 if (thisCurrent != otherCurrent)
 {
 return false;
 }

 H5G_stat_t thisStat, otherStat;

 file.getObjType(thisCurrent, &thisStat);
 other.file.getHDFObjType(thisStat, otherStat);
 }
 }*/
}
