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

#include "DoubleMatrixArray.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace hedge {

DoubleMatrixArray::DoubleMatrixArray() {}

DoubleMatrixArray::DoubleMatrixArray(int rows, int cols, int len)
{
  init(rows, cols, len);
}

DoubleMatrixArray::DoubleMatrixArray(ifstream& matricesFile)
{
  mats.clear();
  vector<vector<double>> currentMatrix;
  string line;

  // Read all double matrices from "matricesFile" into "mats" vector of
  // matrices.
  // Note that "matricesFile" is expected to have the following format:
  // [[mat1_{11}, mat1_{12}, ... , mat1_{1n}]
  // [mat1_{21}, mat1_{22}, ... , mat1_{2n}]
  // ...
  // [mat1_{m1}, mat1_{m2}, ... , mat1_{mn}],
  // [mat2_{11}, mat2_{12}, ... , mat2_{1n}]
  // [mat2_{21}, mat2_{22}, ... , mat2_{2n}]
  // ...
  // [mat2_{m1}, mat2_{m2}, ... , mat2_{mn}],
  // ...
  // [matk_{11}, matk_{12}, ... , matk_{1n}]
  // [matk_{21}, matk_{22}, ... , matk_{2n}]
  // ...
  // [matk_{m1}, matk_{m2}, ... , matk_{mn}]]
  while (getline(matricesFile, line)) {
    currentMatrix.push_back(vector<double>(0));

    bool lastRowInMatrix = false;

    // If "line" ends with new line charecter, remove it.
    // Note that getline() generally eats up newline charecters, but this
    // behaviour may chage,
    // for example, when dealing with files that are moved between different
    // operation systems.
    if ((line.back() == '\n') || (line.back() == '\r')) {
      line.pop_back();
    }

    // Check whether the current array in matricesFile corresponds to the last
    // row in the
    // the current matrix.
    // Note that the last array in each matrix is expected to end at ',' or "]]"
    // charecters.
    if ((line.back() == ',') || (line.substr(line.size() - 2, 2)) == "]]") {
      lastRowInMatrix = true;
    }

    // Read current array in matricesFile into "sstream"
    istringstream sstream(line);
    char nextChar;

    // Ignore '[' charecters in the current array
    while (sstream.peek() == '[') {
      sstream.ignore();
    }

    // Check whether the current array is empty
    if (sstream.peek() == ']') {
      // The current array is empty
      continue;
    }

    string nextVal;

    // Read all doubles from current array
    while (getline(sstream, nextVal, ',')) {
      // If getline() returned extra ']' charecter in the last read, remove ']'
      // charecter
      // from "nextVal".
      // Note that an Extra ']' charecter will be returned when reading the last
      // number in
      // the current array.
      while (nextVal.back() == ']') {
        nextVal.pop_back();
      }

      double doubleVal = stod(nextVal.c_str());

      currentMatrix.back().push_back(doubleVal);

      // Ignore space charecter
      sstream.ignore();
    }

    if (lastRowInMatrix) {
      DoubleMatrix extractedMatrix;
      extractedMatrix.init(currentMatrix);

      pushBackMatrix(extractedMatrix);

      currentMatrix.clear();
    }
  }
}

DoubleMatrixArray::DoubleMatrixArray(
    int len,
    const std::vector<std::vector<double>>& fixedMatrix)
{
  init(len, fixedMatrix);
}

DoubleMatrixArray::DoubleMatrixArray(
    int len,
    const std::vector<std::vector<float>>& fixedMatrix)
{
  init(len, fixedMatrix);
}

DoubleMatrixArray::DoubleMatrixArray(int len, const DoubleMatrix& fixedMatrix)
{
  init(len, fixedMatrix);
}

DoubleMatrixArray::DoubleMatrixArray(
    const boost::numeric::ublas::tensor<double>& tensor)
{
  init(tensor);
}

streamoff DoubleMatrixArray::save(ostream& stream) const
{
  streampos streamStartPos = stream.tellp();

  size_t len = mats.size();

  stream.write(reinterpret_cast<const char*>(&len), sizeof(int));

  for (int i = 0; i < len; i++) {
    getMat(i).save(stream);
  }

  streampos streamEndPos = stream.tellp();

  return streamEndPos - streamStartPos;
}

streamoff DoubleMatrixArray::load(istream& stream)
{
  streampos streamStartPos = stream.tellg();

  int len;

  stream.read(reinterpret_cast<char*>(&len), sizeof(int));

  mats.resize(len);

  for (int i = 0; i < len; i++) {
    getMat(i).load(stream);
  }

  streampos streamEndPos = stream.tellg();

  return streamEndPos - streamStartPos;
}

void DoubleMatrixArray::loadMatrix(int matrixIndex,
                                   const std::string& h5File,
                                   const std::string& path)
{
  if (mats.size() <= matrixIndex) {
    throw invalid_argument("matrix index exceeds number of matrices");
  }

  mats[matrixIndex].load(h5File, path);
}

void DoubleMatrixArray::init(int rows, int cols, int len)
{
  mats.clear();
  for (int i = 0; i < len; ++i) {
    mats.emplace_back(rows, cols);
  }
}

void DoubleMatrixArray::init(
    int len,
    const std::vector<std::vector<double>>& fixedMatrix)
{
  DoubleMatrix dm(fixedMatrix);
  init(len, dm);
}

void DoubleMatrixArray::init(int len, const DoubleMatrix& fixedMatrix)
{
  mats.clear();
  for (int i = 0; i < len; i++) {
    mats.push_back(fixedMatrix); // push_back creates a copy
  }
}

void DoubleMatrixArray::init(int len,
                             const std::vector<std::vector<float>>& fixedMatrix)
{
  DoubleMatrix dm(fixedMatrix);
  init(len, dm);
}

void DoubleMatrixArray::init(
    const boost::numeric::ublas::tensor<double>& tensor)
{
  if (tensor.order() != 3)
    throw runtime_error("tensor is not of a 3D shape");

  for (size_t k = 0; k < tensor.size(2); ++k) {
    vector<vector<double>> mat;

    for (size_t i = 0; i < tensor.size(0); ++i) {
      vector<double> col;
      for (size_t j = 0; j < tensor.size(1); ++j)
        col.push_back(tensor.at(i, j, k));
      mat.push_back(col);
    }

    mats.push_back(mat);
  }
}

boost::numeric::ublas::tensor<double> DoubleMatrixArray::getTensor() const
{
  boost::numeric::ublas::tensor<double> tensor{(long unsigned int)rows(),
                                               (long unsigned int)cols(),
                                               (long unsigned int)size()};
  for (size_t k = 0; k < tensor.size(2); ++k) {
    const DoubleMatrix& mat = getMat(k);
    for (size_t i = 0; i < tensor.size(0); ++i) {
      for (size_t j = 0; j < tensor.size(1); ++j)
        tensor.at(i, j, k) = mat.get(i, j);
    }
  }
  return tensor;
}

DoubleMatrixArray::~DoubleMatrixArray() {}

void DoubleMatrixArray::initFromH5File(int len,
                                       const std::string& h5File,
                                       const std::string& path)
{
  DoubleMatrix dm(h5File, path);
  init(len, dm);
}

DoubleMatrixArray DoubleMatrixArray::getMatrixMultiply(
    const DoubleMatrixArray& other) const
{
  DoubleMatrixArray res;
  testSameSize("matrixMult", other);

  for (size_t i = 0; i < size(); ++i) {
    res.mats.push_back(mats[i].getMultiply(other.mats[i]));
  }
  return res;
}

DoubleMatrixArray DoubleMatrixArray::getTranspose() const
{
  DoubleMatrixArray res;
  for (size_t i = 0; i < size(); ++i) {
    res.mats.push_back(mats[i].getTranspose());
  }
  return res;
}

void DoubleMatrixArray::transpose() { *this = getTranspose(); }

void DoubleMatrixArray::add(const DoubleMatrixArray& other)
{
  testSameSize("add", other);
  for (size_t i = 0; i < size(); ++i) {
    mats[i] += other.mats[i];
  }
}

void DoubleMatrixArray::addAt(double val, int row, int col)
{
  for (size_t i = 0; i < size(); ++i)
    mats[i].addAt(val, row, col);
}

void DoubleMatrixArray::addAt(const DoubleMatrixArray& other,
                              int startRow,
                              int startCol)
{
  testSameSize("addAt", other);
  for (size_t i = 0; i < size(); ++i)
    mats[i].addAt(other.mats[i], startRow, startCol);
}

void DoubleMatrixArray::elementMultiplyAt(const DoubleMatrixArray& other,
                                          int startRow,
                                          int startCol)
{
  for (size_t i = 0; i < size(); ++i)
    mats[i].elementMultiplyAt(other.mats[i], startRow, startCol);
}

void DoubleMatrixArray::sub(const DoubleMatrixArray& other)
{
  testSameSize("sub", other);
  for (size_t i = 0; i < size(); ++i) {
    mats[i] -= other.mats[i];
  }
}

void DoubleMatrixArray::elementMultiply(const DoubleMatrixArray& other)
{
  for (size_t m = 0; m < size(); m++) {
    mats[m].elementMultiply(other.getMat(m));
  }
}

void DoubleMatrixArray::multiplyByScalar(double scalar)
{
  for (size_t i = 0; i < size(); ++i)
    mats[i].multiplyByScalar(scalar);
}

DoubleMatrixArray DoubleMatrixArray::getMultiplyByScalar(double scalar) const
{
  DoubleMatrixArray res(*this);
  res.multiplyByScalar(scalar);
  return res;
}

void DoubleMatrixArray::innerSum()
{
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j) {
      double sum = 0;
      for (size_t d = 0; d < size(); ++d)
        sum += mats[d].get(i, j);
      for (size_t d = 0; d < size(); ++d)
        mats[d].set(i, j, sum);
    }
}

void DoubleMatrixArray::initRandom()
{
  for (size_t i = 0; i < size(); ++i)
    mats[i].initRandom();
}

void DoubleMatrixArray::initHotel()
{
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j)
      for (size_t k = 0; k < size(); ++k)
        mats[k].set(i, j, i * 100 + j * 10 + k);
}

void DoubleMatrixArray::initConst(double v)
{
  for (size_t i = 0; i < size(); ++i)
    mats[i].initConst(v);
}

void DoubleMatrixArray::initRandomSame()
{
  mats[0].initRandom();
  for (size_t i = 1; i < size(); ++i)
    mats[i] = mats[0];
}

double DoubleMatrixArray::testEquals(const std::string& title,
                                     const DoubleMatrixArray& other,
                                     double eps) const
{
  testSameSize("TEST " + title, other);
  testSameDims("TEST " + title, other);

  double maxDiff = 0;
  int elements = 0;
  for (size_t i = 0; i < size(); ++i) {
    double diff = mats[i].getMaxRelDiff(other.mats[i], eps);
    elements += mats[i].size();
    if (diff > eps) {
      cout << title << ", at matrix " << i << " :" << endl;
      mats[i].debugPrint(cout, 1);
      other.mats[i].debugPrint(cout, 1);
      cout << "Rel-Diff=" << diff << " epsilon=" << eps << endl;
      throw runtime_error("test failed");
    }
    maxDiff = max(maxDiff, diff);
  }
  cout << "TEST OK: " << title << " maxDiff=" << maxDiff << " ( " << elements
       << " elements tested)" << endl;
  return maxDiff;
}

double DoubleMatrixArray::testEqualsZero(const std::string& title,
                                         const std::vector<int>& zs,
                                         double eps) const
{

  double totalMaxAbs = 0;
  int elements = 0;
  for (int z : zs) {
    double maxAbs = mats[z].getMaxAbs();
    elements += mats[z].size();
    if (maxAbs > eps) {
      cerr << title << ", at matrix " << z << " :" << endl;
      mats[z].debugPrint(cerr, 1);
      cerr << "Max-abs=" << maxAbs << " epsilon=" << eps << endl;
      throw runtime_error("test failed");
    }
    totalMaxAbs = max(maxAbs, totalMaxAbs);
  }
  cout << "TEST ZERO OK: " << title << " maxAbs=" << totalMaxAbs << " ( "
       << elements << " elements tested)" << endl;
  return totalMaxAbs;
}

void DoubleMatrixArray::testSameAlongDepth(const std::string& title,
                                           double eps) const
{

  double maxDiff = 0;
  int et = 0;
  for (size_t i = 0; i < size(); ++i) {
    double diff = mats[0].getMaxDiff(mats[i]);
    maxDiff = max(maxDiff, diff);
    et += mats[i].size();
  }
  if (maxDiff > eps) {
    cerr << "ERROR: " << title << " maxDiff=" << maxDiff << endl;
    assert(0);
  }
  cout << "OK: " << title << " maxDiff=" << maxDiff << " ( " << et
       << " elements tested)" << endl;
}

void DoubleMatrixArray::testSameSize(const std::string& title,
                                     const DoubleMatrixArray& other) const
{
  if (size() == other.size())
    return;
  cerr << title << ":" << endl;
  debugPrint(cerr, "this", 0);
  cerr << endl;
  other.debugPrint(cerr, "other", 0);
  cerr << endl;
  throw invalid_argument("mismatching sizes");
}

void DoubleMatrixArray::testSameDims(const std::string& title,
                                     const DoubleMatrixArray& other) const
{
  if (rows() == other.rows() && cols() == other.cols())
    return;
  cerr << title << ":" << endl;
  debugPrint(cerr, "this", 0);
  other.debugPrint(cerr, "other", 0);
  throw invalid_argument("mismatching dims");
}

bool DoubleMatrixArray::checkIfEqual(const DoubleMatrixArray& other,
                                     double eps) const
{
  if (!checkIfSameSize(other) || !checkIfSameDims(other)) {
    cout << "different size or dims\n";
    cout << "first size = " << size() << endl;
    cout << "second size = " << other.size() << endl;

    cout << "first dims = " << getMat(0).rows() << ", " << getMat(0).cols()
         << endl;
    cout << "second dims = " << other.getMat(0).rows() << ", "
         << other.getMat(0).cols() << endl;

    return false;
  }

  for (size_t m = 0; m < size(); m++) {
    if (!getMat(m).checkIfEqual(other.getMat(m), eps)) {
      cout << "difference in index " << m << endl;
      return false;
    }
  }

  return true;
}

bool DoubleMatrixArray::checkIfSameSize(const DoubleMatrixArray& other) const
{
  return (size() == other.size());
}

bool DoubleMatrixArray::checkIfSameDims(const DoubleMatrixArray& other) const
{
  if ((size() == 0) && (other.size() != 0)) {
    return false;
  }

  if ((other.size() == 0) && (size() != 0)) {
    return false;
  }

  return getMat(0).checkIfSameSize(other.getMat(0));
}

void DoubleMatrixArray::debugPrint(std::ostream& out,
                                   const std::string& title,
                                   int verboseLevel) const
{
  out << title << " ";
  if (size() == 0) {
    out << "Empty" << endl;
    return;
  }
  out << "[" << size() << "]: " << getMat(0).rows() << " x "
      << getMat(0).cols();
  if (verboseLevel == 0)
    return;
  out << endl;
  for (size_t i = 0; i < size(); ++i) {
    out << "--- MAT " << i << endl;
    getMat(i).debugPrint(out, verboseLevel);
  }
}

std::ostream& operator<<(std::ostream& out, const DoubleMatrixArray& dma)
{
  dma.debugPrint(out, "DoubleMatrixArray", 0);
  return out;
}

void DoubleMatrixArray::pushBackMatrix(const DoubleMatrix& mat)
{
  if (size() != 0) {
    if ((mat.rows() != getMat(0).rows()) || (mat.cols() != getMat(0).cols())) {
      cerr << "Trying to append to " << *this << " a matrix of size "
           << mat.rows() << "x" << mat.cols() << endl;
      throw invalid_argument("mismatching sizes");
    }
  }

  mats.push_back(mat);
}

void DoubleMatrixArray::square() { elementMultiply(*this); }

void DoubleMatrixArray::clear() { mats.clear(); }

DoubleMatrixArray DoubleMatrixArray::depthSlice(int start, int end) const
{
  DoubleMatrixArray res;
  res.mats = vector<DoubleMatrix>(mats.begin() + start, mats.begin() + end);
  return res;
}

DoubleMatrixArray DoubleMatrixArray::getConvolution(
    const DoubleMatrixArray& filter,
    int strideRows,
    int strideCols) const
{
  testSameSize("conv", filter);
  DoubleMatrixArray res;
  for (size_t i = 0; i < size(); ++i)
    res.mats.push_back(
        mats[i].getConvolution(filter.mats[i], strideRows, strideCols));
  return res;
}

/** Appends given matrix to the right of this matrix */
void DoubleMatrixArray::appendCols(const DoubleMatrixArray& other)
{
  if (rows() == 0 && cols() == 0 && size() == 0)
    mats = vector<DoubleMatrix>(other.size());
  else
    testSameSize("appendCol", other);
  for (size_t i = 0; i < size(); ++i)
    mats[i].appendCols(other.mats[i]);
}

/** Appends given matrix to the bottom of this matrix */
void DoubleMatrixArray::appendRows(const DoubleMatrixArray& other)
{
  if (rows() == 0 && cols() == 0 && size() == 0)
    mats = vector<DoubleMatrix>(other.size());
  else
    testSameSize("appendCol", other);
  for (size_t i = 0; i < size(); ++i)
    mats[i].appendRows(other.mats[i]);
}

DoubleMatrixArray DoubleMatrixArray::getSubMatrix(int row1,
                                                  int col1,
                                                  int row2,
                                                  int col2) const
{
  DoubleMatrixArray res;
  res.mats = vector<DoubleMatrix>(size());
  for (size_t i = 0; i < size(); ++i)
    res.mats[i] = mats[i].getSubMatrix(row1, col1, row2, col2);

  return res;
}

DoubleMatrixArray DoubleMatrixArray::getSubMatrix(int row, int col) const
{
  return getSubMatrix(row, col, row + 1, col + 1);
}

DoubleMatrixArray DoubleMatrixArray::getColumn(int col) const
{
  return getSubMatrix(0, col, rows(), col + 1);
}

DoubleMatrixArray DoubleMatrixArray::getMeanAlongRows() const
{
  DoubleMatrixArray res;
  for (size_t i = 0; i < mats.size(); i++) {
    res.pushBackMatrix(mats[i].getMeanAlongRows());
  }

  return res;
}
DoubleMatrixArray DoubleMatrixArray::getMeanAlongCols() const
{
  DoubleMatrixArray res;
  for (size_t i = 0; i < mats.size(); i++) {
    res.pushBackMatrix(mats[i].getMeanAlongCols());
  }

  return res;
}
DoubleMatrixArray DoubleMatrixArray::getMeanInDepth() const
{
  size_t sizeOrig = mats.size();

  DoubleMatrixArray res = getSumInDepth();
  res.multiplyByScalar((double)1 / sizeOrig);

  return res;
}

DoubleMatrixArray DoubleMatrixArray::getSumAlongRows() const
{
  DoubleMatrixArray res;
  for (size_t i = 0; i < mats.size(); i++) {
    res.pushBackMatrix(mats[i].getSumAlongRows());
  }

  return res;
}

DoubleMatrixArray DoubleMatrixArray::getSumAlongCols() const
{
  DoubleMatrixArray res;
  for (size_t i = 0; i < mats.size(); i++) {
    res.pushBackMatrix(mats[i].getSumAlongCols());
  }

  return res;
}
DoubleMatrixArray DoubleMatrixArray::getSumInDepth() const
{
  DoubleMatrixArray res;

  if (size() == 0) {
    return res;
  }

  res.pushBackMatrix(mats[0]);

  for (size_t i = 1; i < mats.size(); i++) {
    res.mats[0].addAt(mats[i], 0, 0);
  }

  return res;
}

void DoubleMatrixArray::meanAlongRows() { *this = getMeanAlongRows(); }
void DoubleMatrixArray::meanAlongCols() { *this = getMeanAlongCols(); }
void DoubleMatrixArray::meanInDepth() { *this = getMeanInDepth(); }
void DoubleMatrixArray::sumAlongRows() { *this = getSumAlongRows(); }
void DoubleMatrixArray::sumAlongCols() { *this = getSumAlongCols(); }
void DoubleMatrixArray::sumInDepth() { *this = getSumInDepth(); }

void DoubleMatrixArray::setInAllDepth(int i, int j, double val)
{
  for (size_t k = 0; k < size(); ++k)
    mats[k].set(i, j, val);
}

vector<double> DoubleMatrixArray::getInAllDepth(int i,
                                                int j,
                                                double scale) const
{
  vector<double> res;
  for (size_t k = 0; k < size(); ++k)
    res.push_back(getMat(k).get(i, j) * scale);
  return res;
}

void DoubleMatrixArray::flattenIntoVec(vector<double>& vec) const
{
  for (const DoubleMatrix& mat : mats) {
    mat.flattenIntoVec(vec);
  }
}
}
