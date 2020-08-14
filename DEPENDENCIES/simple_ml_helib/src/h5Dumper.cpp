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

#include "h5Dumper.h"

using namespace std;
using namespace H5;

H5Dumper::H5Dumper(const string& f) {
  this->file_name = f;
  file = H5File(f, H5F_ACC_TRUNC);
}

H5Dumper::~H5Dumper() {
  file.close();
}

bool H5Dumper::objectExists(const string& name) const {
  try {
    H5G_stat_t dummy;
    H5::Exception::dontPrint();
    file.getObjinfo(name, dummy);
    return true;
  } catch (H5::Exception&) {
    return false;
  }
}

void H5Dumper::setLayerNames(const vector<string>& layerNames)
{
  hsize_t dims[1] = { layerNames.size() };
  DataSpace dataSpace(1, dims);
  StrType strDataType(PredType::C_S1, H5T_VARIABLE);
  vector<const char*> layerNamesCStr(layerNames.size());

  for(size_t i = 0; i < layerNames.size(); i++)
  {
    layerNamesCStr[i] = layerNames[i].c_str();
  }

  Attribute attribute = file.createAttribute("layer_names", strDataType, dataSpace);

  attribute.write(strDataType, layerNamesCStr.data());
}

void H5Dumper::setLayerWeightNames(const string& layerName,
    const vector<string>& weightNames) {
  Group group;
  if (objectExists(layerName))
  {
    group = file.openGroup(layerName);
  }
  else
  {
    group = file.createGroup(layerName);
  }

  hsize_t dims[1] = { weightNames.size() };
  DataSpace dataSpace(1, dims);
  StrType strDataType(PredType::C_S1, H5T_VARIABLE);
  vector<const char*> weightNamesCStr(weightNames.size());

  for(size_t i = 0; i < weightNames.size(); i++)
  {
    weightNamesCStr[i] = weightNames[i].c_str();
  }

  Attribute attribute = group.createAttribute("weight_names", strDataType, dataSpace);

  attribute.write(strDataType, weightNamesCStr.data());
}

DataSet H5Dumper::createDataSet(const DataSpace& dataSpace,
      const DataType& dataType, const string& dataSetName) {
  if (objectExists(dataSetName)) {
    return file.openDataSet(dataSetName);
  }
  else
  {
    return file.createDataSet(dataSetName, dataType, dataSpace);
  }
}

DataSet H5Dumper::createDataSet(Group& group, const DataSpace& dataSpace,
      const DataType& dataType, const string& dataSetName) {
  string path = group.getObjName() + "/" + dataSetName;

  if (objectExists(path)) {
    return group.openDataSet(dataSetName);
  }
  else
  {
    return group.createDataSet(dataSetName, dataType, dataSpace);
  }
}

Group H5Dumper::createGroup(const std::string& groupName)
{
  if (objectExists(groupName)) {
    return file.openGroup(groupName);
  }
  else
  {
    return file.createGroup(groupName);
  }
}

Group H5Dumper::createSubGroup(Group& parent, const std::string& groupName)
{
  string path = parent.getObjName() + "/" + groupName;

  if (objectExists(path)) {
    return parent.openGroup(groupName);
  }
  else
  {
    return parent.createGroup(groupName);
  }
}

void H5Dumper::dumpBias(const vector<double>& bias, const string& layerName) {
  hsize_t dims[1] = { bias.size() };
  DataSpace dataSpace(1, dims);
  DataType dataType(PredType::IEEE_F64LE);

  Group group = createGroup(layerName);

  Group subGroup = createSubGroup(group, layerName);

  DataSet dataSet = createDataSet(subGroup, dataSpace, dataType, "bias:0");

  dataSet.write(bias.data(), PredType::IEEE_F64LE);
}

void H5Dumper::dumpFC(const vector<vector<double> >& weights,
    const string& layerName) {
  hsize_t dims[2] = { weights.size(), weights[0].size() };
  DataSpace dataSpace(2, dims);
  DataType dataType(PredType::IEEE_F32LE);

  Group group = createGroup(layerName);

  Group subGroup = createSubGroup(group, layerName);

  DataSet dataSet = createDataSet(subGroup, dataSpace, dataType, "kernel:0");

  // For compatibility with Keras, we dump the weights as floats rather than doubles
  vector<float> weights1D(0);
  int cnt = 0;

  for (int i = 0; i < weights.size(); i++) {
    for (int j = 0; j < weights[i].size(); j++) {
      weights1D.push_back(weights[i][j]);
    }
  }

  dataSet.write(weights1D.data(), PredType::IEEE_F32LE);
}

void H5Dumper::dumpData(const vector<double>& data, const vector<int>& dims,
    const string& dataSetName) {
  vector<hsize_t> dimsCopy(dims.size());

  for (int i = 0; i < dims.size(); i++) {
    dimsCopy[i] = dims[i];
  }

  DataSpace dataSpace(dimsCopy.size(), dimsCopy.data());
  DataType dataType(PredType::NATIVE_DOUBLE);

  DataSet dataSet = createDataSet(dataSpace, dataType, dataSetName);

  dataSet.write(data.data(), PredType::NATIVE_DOUBLE);
}


void H5Dumper::dumpFilters(const vector<vector<vector<double> > >& filters,
    const string& layerName) {
  hsize_t dims[4] = { filters[0].size(), filters[0][0].size(), 1, filters.size()};
  DataSpace dataSpace(4, dims);
  DataType dataType(PredType::IEEE_F32LE);

  Group group = createGroup(layerName);

  Group subGroup = createSubGroup(group, layerName);

  DataSet dataSet = createDataSet(subGroup, dataSpace, dataType, "kernel:0");

  // For compatibility with Keras, we dump the filters as floats rather than doubles
  vector<float> filters1D(0);
  int cnt = 0;

  // Push all filters into the one-dimensional vector "filters1D".
  // Note that the first dimension of "filters" vector is for the filters' rows, the second
  // is for the filters' columns, and the third is for the different filters.
  // For compatibility with Keras h5 output files, we first iterate on the rows dimension, then on
  // the columns dimension and lastly on the different filters.
  for (int i = 0; i < filters[0].size(); i++) {
    for (int j = 0; j < filters[0][i].size(); j++) {
      for (int k = 0; k < filters.size(); k++)
      {
        filters1D.push_back(filters[k][i][j]);
      }
    }
  }

  dataSet.write(filters1D.data(), PredType::IEEE_F32LE);
}

void H5Dumper::dumpEmptyLayer(const string& layerName) {
  createGroup(layerName);
}
