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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "DoubleMatrix.h"
#include "helayers/hebase/hebase.h"
#include <stdlib.h>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "h5Parser.h"

using namespace std;

namespace helayers {

DoubleMatrix::~DoubleMatrix()
{
  // TODO Auto-generated destructor stub
}

DoubleMatrix::DoubleMatrix() : m(0, 0) {}

DoubleMatrix::DoubleMatrix(int r, int c, double fixedVal) : m(r, c, fixedVal) {}

DoubleMatrix::DoubleMatrix(const std::vector<double> v, int matrixRows)
{
  int matrixCols = (v.size() + matrixRows - 1) / matrixRows;
  m.resize(matrixRows, matrixCols);
  for (size_t i = 0; i < v.size(); ++i)
    set(i / cols(), i % cols(), v[i]);
}

DoubleMatrix::DoubleMatrix(ifstream& matrixFile)
{
  // TODO get rid of inputVals, load straight into matrix
  vector<vector<double>> inputVals(0);
  string line;

  // Read all double arrays from "matrixFile" into "inputVals" matrix.
  // Note that "matrixFile" is expected to have the following format:
  // [vals_{11}, vals_{12}, ... , vals_{1n}]
  // [vals_{21}, vals_{22}, ... , vals_{2n}]
  // ...
  // [vals_{m1}, vals_{m2}, ... , vals_{mn}].
  while (getline(matrixFile, line)) {
    inputVals.push_back(vector<double>(0));

    // Read current array in matrix file into "sstream"
    istringstream sstream(line);
    char nextChar;

    // Ignore '[' charecter in the current array in "matrixFile".
    // Note that '[' charecter is expected to appear at the begining of every
    // array.
    sstream.ignore();

    // Check whether the current array is empty
    nextChar = sstream.peek();

    if (nextChar == ']') {
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
      if (nextVal.back() == ']') {
        nextVal.pop_back();
      }

      double doubleVal = stod(nextVal.c_str());

      inputVals.back().push_back(doubleVal);

      // Ignore space charecter
      sstream.ignore();
    }
  }

  init(inputVals);
}

DoubleMatrix::DoubleMatrix(const std::vector<std::vector<float>>& matrix)
{
  init(matrix);
}

DoubleMatrix::DoubleMatrix(const std::vector<std::vector<double>>& matrix)
{
  init(matrix);
}

DoubleMatrix::DoubleMatrix(const std::vector<std::vector<double>>& matrix,
                           int r,
                           int c,
                           double fixedVal)
    : m(r, c, fixedVal)
{
  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = 0; j < matrix[0].size(); ++j) {
      m(i, j) = static_cast<double>(matrix[i][j]);
    }
  }
}

DoubleMatrix::DoubleMatrix(const string& h5File, const string& path)
{
  load(h5File, path);
}

DoubleMatrix::DoubleMatrix(const boost::numeric::ublas::tensor<double>& t)
{
  if (t.order() != 2)
    throw runtime_error(
        "DoubleMatrix can be initialized only from a 2-dimensional tensor");
  auto shape = t.extents();
  m.resize(t.size(0), t.size(1), false);
  for (size_t i = 0; i < t.size(0); ++i)
    for (size_t j = 0; j < t.size(1); ++j)
      m(i, j) = t.at(i, j);
}

void DoubleMatrix::load(const string& h5File, const string& path)
{
  H5Parser h5P(h5File);

  // Initialize "vals" member matrix with the corresponding matrix provided in
  // the h5 file
  init(h5P.parseFC(path));

  // Transpose "vals" member matrix, after reading it from the h5 file provided
  // by Keras.
  // Note that when Dense layers matrices are used in multiplications in Keras,
  // the weights
  // matrix is in the right side of the multiplication. While in prodcuts
  // executed by nn_train,
  // the weights matrix is in the left side.
  // Therefore, we transpose the weights matrix provided by Keras in order to
  // ensure the results
  // of all multiplications will be the same (up to transposition).
  transpose();
}

std::vector<std::vector<double>> DoubleMatrix::getVals() const
{
  vector<vector<double>> res(rows());
  for (int i = 0; i < rows(); ++i) {
    res[i].resize(cols());
    for (int j = 0; j < cols(); ++j) {
      res[i][j] = m(i, j);
    }
  }
  return res;
}

boost::numeric::ublas::tensor<double> DoubleMatrix::getTensor() const
{
  boost::numeric::ublas::tensor<double> res{(long unsigned int)rows(),
                                            (long unsigned int)cols()};
  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j) {
      res.at(i, j) = m(i, j);
    }
  }
  return res;
}

void DoubleMatrix::elementMultiply(const DoubleMatrix& other)
{
  always_assert(rows() == other.rows());
  always_assert(cols() == other.cols());
  m = boost::numeric::ublas::element_prod(m, other.m);
}

void DoubleMatrix::elementMultiplyAt(const DoubleMatrix& other,
                                     int startRow,
                                     int startCol)
{
  for (int i = 0; i < other.rows(); ++i) {
    for (int j = 0; j < other.cols(); ++j) {
      m(startRow + i, startCol + j) *= other.m(i, j);
    }
  }
}

void DoubleMatrix::meanAlongRows() { *this = getMeanAlongRows(); }

DoubleMatrix DoubleMatrix::getMeanAlongRows() const
{
  int rowsOrig = rows();
  DoubleMatrix dm = getSumAlongRows();
  dm.multiplyByScalar((double)1 / rowsOrig);
  return dm;
}

void DoubleMatrix::meanAlongCols() { *this = getMeanAlongCols(); }

DoubleMatrix DoubleMatrix::getMeanAlongCols() const
{
  int colsOrig = cols();
  DoubleMatrix dm = getSumAlongCols();
  dm.multiplyByScalar((double)1 / colsOrig);
  return dm;
}

void DoubleMatrix::sumAlongRows() { *this = getSumAlongRows(); }

DoubleMatrix DoubleMatrix::getSumAlongRows() const
{
  if (rows() == 0)
    throw runtime_error("empty matrix");
  DoubleMatrix res(1, m.size2());
  for (int j = 0; j < cols(); ++j) {
    double sum = 0;
    for (int i = 0; i < rows(); ++i) {
      sum += get(i, j);
    }
    res.set(0, j, sum);
  }

  return res;
}

void DoubleMatrix::sumAlongCols() { *this = getSumAlongCols(); }

DoubleMatrix DoubleMatrix::getSumAlongCols() const
{
  if (cols() == 0) {
    throw invalid_argument("empty matrix");
  }
  DoubleMatrix res(rows(), 1);
  for (int i = 0; i < rows(); ++i) {
    double sum = 0;
    for (int j = 0; j < cols(); ++j) {
      sum += get(i, j);
    }
    res.set(i, 0, sum);
  }

  return res;
}
double DoubleMatrix::getMaxDiff(const DoubleMatrix& other) const
{
  if (size() == 0)
    throw invalid_argument("empty matrix");
  double res = 0;
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j)
      res = max(res, abs(get(i, j) - other.get(i, j)));
  return res;
}

double DoubleMatrix::getMaxRelDiff(const DoubleMatrix& other,
                                   double tolerance) const
{
  if (size() == 0)
    throw invalid_argument("empty matrix");
  double res = 0;
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j) {
      double v1 = get(i, j);
      double v2 = other.get(i, j);
      double diff = abs(v1 - v2);
      double m = max(abs(v1), abs(v2));
      double relDiff = m < (tolerance * 10) ? diff : (diff / m);
      res = max(res, relDiff);
    }
  return res;
}

double DoubleMatrix::getMaxAbs() const
{
  if (size() == 0)
    throw invalid_argument("empty matrix");
  double res = 0;
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j)
      res = max(get(i, j), res);
  return res;
}

DoubleMatrix DoubleMatrix::getMultiply(const DoubleMatrix& other) const
{
  if (cols() != other.rows()) {
    cerr << "Can't multiply: " << endl;
    debugPrint(cerr, 0);
    other.debugPrint(cerr, 0);
    throw invalid_argument("mismatching dims");
  }
  DoubleMatrix res(rows(), other.cols());
  res.m = boost::numeric::ublas::prod(m, other.m);
  return res;
}

/*DoubleMatrixArray DoubleMatrix::getConvolution(const DoubleMatrixArray&
filters,
    int strideRows, int strideCols) const {
  DoubleMatrixArray res;

  for (size_t f = 0; f < filters.size(); f++) {
    DoubleMatrix currentFilterConvRes = getConvolution(filters.getMat(f),
        strideRows, strideCols);

    res.pushBackMatrix(currentFilterConvRes);
  }

  return res;
}*/

DoubleMatrix DoubleMatrix::getConvolution(const DoubleMatrix& filter,
                                          int strideRows,
                                          int strideCols) const
{
  int filterNumRows = filter.rows();
  int filterNumCols = filter.cols();

  DoubleMatrix res(ceil((double)(rows() - filterNumRows) / strideRows) + 1,
                   ceil((double)(cols() - filterNumCols) / strideCols) + 1);

  for (int r = 0, tr = 0; r + filterNumRows - strideRows < rows();
       r += strideRows, tr++) {
    for (int c = 0, tc = 0; c + filterNumCols - strideCols < cols();
         c += strideCols, tc++) {
      double currentInnerProduct = 0;
      for (int i = 0; i < filterNumRows; i++) {
        for (int j = 0; j < filterNumCols; j++) {
          if (r + i >= rows() || c + j >= cols())
            continue;
          currentInnerProduct += (filter.get(i, j) * get(i + r, j + c));
        }
      }
      res.set(tr, tc, currentInnerProduct);
    }
  }

  return res;
}

DoubleMatrix DoubleMatrix::getSubMatrix(int row1,
                                        int col1,
                                        int row2,
                                        int col2) const
{
  if ((row1 < 0) || (row1 >= row2) || (row2 > rows()) || (col1 < 0) ||
      (col1 >= col2) || (col2 > cols())) {
    cerr << "Requesting range: (" << row1 << "," << col1 << ")-(" << row2 << ","
         << col2 << ")" << endl;
    cerr << "Out of this: " << *this << endl;
    throw invalid_argument("out of range");
  }

  DoubleMatrix res(row2 - row1, col2 - col1);
  for (int i = row1; i < row2; ++i)
    for (int j = col1; j < col2; ++j)
      res.set(i - row1, j - col1, get(i, j));
  // TODO consider matrix subslice
  return res;
}

void DoubleMatrix::multiply(const DoubleMatrix& other)
{
  *this = getMultiply(other);
}

void DoubleMatrix::appendRow(const std::vector<double>& vec)
{
  if ((vec.size() != cols()) && (rows() != 0)) {
    throw invalid_argument("mismatching dims");
  }

  int newCols = rows() == 0 ? vec.size() : cols();
  m.resize(rows() + 1, newCols, true);
  boost::numeric::ublas::matrix_row<double_matrix_t> lastRow(m, rows() - 1);
  for (size_t i = 0; i < lastRow.size(); ++i) {
    lastRow(i) = vec[i];
  }
}

void DoubleMatrix::appendCols(const DoubleMatrix& other)
{
  if (cols() != 0 && other.rows() != rows())
    throw invalid_argument("mismatching dims");
  int origCols = cols();
  m.resize(other.rows(), cols() + other.cols(), true);
  for (int i = 0; i < other.rows(); ++i) {
    for (int j = 0; j < other.cols(); ++j) {
      m(i, origCols + j) = other.get(i, j);
    }
  }
}

void DoubleMatrix::appendRows(const DoubleMatrix& other)
{
  if (rows() != 0 && other.cols() != cols())
    throw invalid_argument("mismatching dims");
  int origRows = rows();
  m.resize(rows() + other.rows(), other.cols(), true);
  for (int i = 0; i < other.rows(); ++i) {
    for (int j = 0; j < other.cols(); ++j) {
      m(origRows + i, j) = other.get(i, j);
    }
  }
}

void DoubleMatrix::init(std::vector<std::vector<float>> matrix)
{
  m.resize(matrix.size(), matrix[0].size(), false);
  for (size_t i = 0; i < m.size1(); ++i) {
    for (size_t j = 0; j < m.size2(); ++j) {
      m(i, j) = static_cast<double>(matrix[i][j]);
    }
  }
}

void DoubleMatrix::init(std::vector<std::vector<double>> matrix)
{
  m.resize(matrix.size(), matrix[0].size(), false);
  for (size_t i = 0; i < m.size1(); ++i) {
    for (size_t j = 0; j < m.size2(); ++j) {
      m(i, j) = matrix[i][j];
    }
  }
}

void DoubleMatrix::initRandom()
{
  for (size_t i = 0; i < m.size1(); ++i) {
    for (size_t j = 0; j < m.size2(); ++j) {
      int val = rand() % 10000;
      double dval = ((double)val / 10000.0) * 2 - 1;
      m(i, j) = dval / m.size1();
    }
  }
}

void DoubleMatrix::initConst(double v)
{
  std::fill(m.data().begin(), m.data().end(), v);
}

streamoff DoubleMatrix::save(ostream& stream) const
{
  streampos streamStartPos = stream.tellp();

  const int r = rows();
  stream.write(reinterpret_cast<const char*>(&r), sizeof(int));

  const int c = cols();
  stream.write(reinterpret_cast<const char*>(&c), sizeof(int));

  for (int i = 0; i < rows(); i++) {
    for (int j = 0; j < cols(); j++) {
      const double d = get(i, j);
      stream.write(reinterpret_cast<const char*>(&d), sizeof(double));
    }
  }

  streampos streamEndPos = stream.tellp();

  return streamEndPos - streamStartPos;
}

streamoff DoubleMatrix::load(istream& stream)
{
  streampos streamStartPos = stream.tellg();

  int r = 0;
  stream.read(reinterpret_cast<char*>(&r), sizeof(int));

  int c = 0;
  stream.read(reinterpret_cast<char*>(&c), sizeof(int));

  m.resize(r, c);

  for (int i = 0; i < rows(); i++) {
    for (int j = 0; j < cols(); j++) {
      double d = 0;
      stream.read(reinterpret_cast<char*>(&d), sizeof(double));
      set(i, j, d);
    }
  }

  streampos streamEndPos = stream.tellg();

  return streamEndPos - streamStartPos;
}

void DoubleMatrix::flattenIntoVec(std::vector<double>& vv) const
{
  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j) {
      vv.push_back(get(i, j));
    }
  }
}

std::vector<double> DoubleMatrix::getFlatten() const
{
  vector<double> res;
  flattenIntoVec(res);
  return res;
}

void DoubleMatrix::unFlatten(const std::vector<double>& v)
{
  if (v.size() != size()) {
    throw invalid_argument("mismatching dims");
  }

  int pos = 0;
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j)
      set(i, j, v[pos++]);
}

void DoubleMatrix::debugPrint(ostream& out, int verboseLevel) const
{
  out << "Matrix " << rows() << " x " << cols() << endl;
  if (verboseLevel == 0)
    return;
  const int precision = verboseLevel > 1 ? 10 : 3;
  out << fixed;
  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j)
      out << setw(precision + 6) << setprecision(precision) << get(i, j);
    out << endl;
  }
  out << defaultfloat;
}

ostream& operator<<(ostream& out, const DoubleMatrix& dm)
{
  dm.debugPrint(out, 1);
  return out;
}

void DoubleMatrix::multiplyByScalar(double scalar)
{
  m *= scalar;
  // for (int i = 0; i < rows(); ++i)
  //  for (int j = 0; j < cols(); ++j)
  //    set(i, j, get(i, j) * scalar);
}

DoubleMatrix DoubleMatrix::getTranspose() const
{
  DoubleMatrix res(cols(), rows());
  for (int i = 0; i < rows(); ++i)
    for (int j = 0; j < cols(); ++j)
      res.set(j, i, get(i, j));
  res.m = boost::numeric::ublas::trans(m);
  return res;
}

void DoubleMatrix::transpose() { *this = getTranspose(); }

void DoubleMatrix::testEqual(const string& title,
                             const DoubleMatrix& other,
                             double tolerance) const
{
  testSameSize(title, other);
  double maxDiff = getMaxRelDiff(other, tolerance);
  if (maxDiff < tolerance) {
    cout << title << " OK (diff=" << maxDiff << ")" << endl;
  } else {
    cout << title << endl;
    cout << "EXP: " << *this;
    cout << "ACT: " << other;
    cout << "rel-diff: " << maxDiff << endl;
    cout << "abs-diff: " << getMaxDiff(other) << endl;
    throw runtime_error("test failed");
  }
}

void DoubleMatrix::testSameSize(const string& title,
                                const DoubleMatrix& other) const
{
  if (other.rows() == rows() && other.cols() == cols())
    return;
  cerr << title << ":" << endl;
  debugPrint(cerr, 0);
  other.debugPrint(cerr, 0);
  throw invalid_argument("mismatching sizes");
}

bool DoubleMatrix::checkIfEqual(const DoubleMatrix& other,
                                double tolerance) const
{
  if (!checkIfSameSize(other)) {
    cout << "different dims" << endl;
    return false;
  }

  for (int i = 0; i < rows(); i++) {
    for (int j = 0; j < cols(); j++) {
      const double diff = fabs(get(i, j) - other.get(i, j));
      if (diff >= tolerance) {
        cout << "difference in indexes " << i << ", " << j << endl;
        cout << "first val = " << get(i, j) << endl;
        cout << "second val = " << other.get(i, j) << endl;
        cout << "diff = " << std::setprecision(9) << diff << endl;
        cout << "tolerance = " << std::setprecision(9) << tolerance << endl;
        return false;
      }
    }
  }

  return true;
}

bool DoubleMatrix::checkIfSameSize(const DoubleMatrix& other) const
{
  return ((rows() == other.rows()) && (cols() == other.cols()));
}

void DoubleMatrix::copy(const DoubleMatrix& other)
{
  testSameSize("copy", other);
  m = other.m;
}

DoubleMatrix& DoubleMatrix::operator-=(const DoubleMatrix& other)
{
  testSameSize("-=", other);
  m -= other.m;
  return *this;
}

DoubleMatrix& DoubleMatrix::operator+=(const DoubleMatrix& other)
{
  testSameSize("+=", other);
  m += other.m;
  return *this;
}

void DoubleMatrix::addAt(const DoubleMatrix& other, int startRow, int startCol)
{
  if ((startRow < 0) || (startCol < 0) || (startRow + other.rows() > rows()) ||
      (startCol + other.cols() > cols())) {
    throw invalid_argument("mismatching dims");
  }
  // TODO maybe matrix range
  for (int i = 0; i < other.rows(); ++i) {
    for (int j = 0; j < other.cols(); ++j) {
      m(startRow + i, startCol + j) += other.get(i, j);
    }
  }
}

DoubleMatrix DoubleMatrix::getDuplicate(int dupRows, int dupCols) const
{
  DoubleMatrix res(rows() * dupRows, cols() * dupCols);
  for (int i = 0; i < res.rows(); ++i)
    for (int j = 0; j < res.cols(); ++j)
      res.set(i, j, get(i / dupRows, j / dupCols));
  return res;
}

string DoubleMatrix::niceFormat(double v)
{
  if (abs(v) < 1e-8)
    return "[0]";
  ostringstream out;
  out.precision(12);
  out << std::fixed << v;
  return out.str();
  // return to_string(v);
}

void DoubleMatrix::clear() { m.resize(0, 0, false); }

int DoubleMatrix::argmax() const
{
  if (rows() == 0 || cols() == 0)
    throw runtime_error("Empty matrix");
  if (rows() > 1 && cols() > 1)
    throw runtime_error("Argmax is only supported on 1xn or nx1 matrix");

  double max = get(0, 0);
  int argmax = 0;
  int currIndex = 0;
  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j) {
      double val = get(i, j);
      if (val >= max) {
        max = val;
        argmax = currIndex;
      }
      currIndex++;
    }
  }
  return argmax;
}
} // namespace helayers
