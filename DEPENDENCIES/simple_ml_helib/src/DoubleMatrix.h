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

#ifndef DOUBLEMATRIX_H_
#define DOUBLEMATRIX_H_

#include <vector>
#include <iostream>
#include <fstream>
#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>
#include <boost/numeric/ublas/matrix.hpp>
//#include "norms.h"

class DoubleMatrixArray;

using double_matrix_t = boost::numeric::ublas::matrix<double>;

class DoubleMatrix {

  double_matrix_t m;
public:

  DoubleMatrix();
  DoubleMatrix(int r, int c, double fixedVal = 0);
  DoubleMatrix(std::ifstream& matrixFile);
  DoubleMatrix(const std::string& h5File, const std::string& path);
  DoubleMatrix(std::vector<double> v, int matrixRows);
  DoubleMatrix(const std::vector<std::vector<float> >& matrix);
  DoubleMatrix(const std::vector<std::vector<double> >& matrix);
  DoubleMatrix(const std::vector<std::vector<double> >& matrix, int r, int c, double fixedVal = 0);
  virtual ~DoubleMatrix();

  std::streamoff save(std::ostream& stream) const;
  std::streamoff load(std::istream& stream);
  void load(const std::string& h5File, const std::string& path);

  std::vector<std::vector<double> > getVals() const;

  inline double get(int i, int j) const {
    return m(i, j);
  }

  inline double set(int i, int j, double val) {
    return m(i, j) = val;
  }

  inline int rows() const {
    return m.size1();
  }
  inline int cols() const {
    return m.size2();
  }

  DoubleMatrix& operator-=(const DoubleMatrix& other);
  DoubleMatrix& operator+=(const DoubleMatrix& other);

  // add a value to given position
  inline void addAt(double val, int i, int j) {
    m(i, j) += val;
  }
  // add a smaller matrix other at position startRow,startCol
  void addAt(const DoubleMatrix&other,int startRow,int startCol);

  DoubleMatrix getTranspose() const;

  // returns a sub matrix
  // end indices are exclusive
  DoubleMatrix getSubMatrix(int row1,int col1,int row2,int col2) const;

  void transpose();

  // Makes sure that all the elements in this DoubleMatrix are equal to the elements of "other", up
  // to difference which is less than "tolerance".
  // This function fails if the two matrices are not equal
  void testEqual(const std::string& title, const DoubleMatrix&other,
      double tolerance = 1e-6) const;

  // Makes sure that this DoubleMatrix has the same dimensions as the dimensions of "other".
  // This function fails if the two matrices have different dimensions.
  void testSameSize(const std::string& title, const DoubleMatrix&other) const;

  // Returns a boolean specifying whether all the elements in this DoubleMatrix are equal to the
  // elements of "other", up to difference which is less than "tolerance"
  bool checkIfEqual(const DoubleMatrix&other, double tolerance = 1e-6) const;

  // Returns a boolean specifying whether this DoubleMatrix has the same dimensions as the
  // dimensions of "other".
  bool checkIfSameSize(const DoubleMatrix&other) const;

  // verifies other has same size, and copies its values
  void copy(const DoubleMatrix&other);

  void init(const std::vector<std::vector<float> > matrix);
  void init(const std::vector<std::vector<double> > matrix);
  void initRandom();
  void initConst(double v);

  double getMaxDiff(const DoubleMatrix&other) const;

  /** returns maximal absolute value */
  double getMaxAbs() const;

  // Computes the maximal difference relative to values tested:
  // For each pair of compared values v1 and v2:
  // if max(abs(v1),abs(v2))<10*tolerance, then the diff is abs(v1-v2)
  // otherwise, abs(v1-v2)/max(abs(v1),abs(v2))
  double getMaxRelDiff(const DoubleMatrix&other, double tolerance) const;

  void flattenIntoVec(std::vector<double>& vals) const;

  // Returns the result of multiplying the stored matrix with "other".
  DoubleMatrix getMultiply(const DoubleMatrix& other) const;

  // Updates the stored matrix to the result of the multiplication of its previous value with
  // "other"
  void multiply(const DoubleMatrix& other);

  void elementMultiply(const DoubleMatrix& other);
  void elementMultiplyAt(const DoubleMatrix& other,int startRow,int startCol);
  void multiplyByScalar(double scalar);

  // reduces the rows to a single row by calculating the mean of each column
  void meanAlongRows();

  // reduces the columns to a single column by calculating the mean of each row
  void meanAlongCols();

  // reduces the rows to a single row by calculating the sum of each column
  void sumAlongRows();

  // reduces the columns to a single column by calculating the sum of each row
  void sumAlongCols();

  DoubleMatrix getDuplicate(int dupRows, int dupCols) const;

  std::vector<double> getFlatten() const;
  void unFlatten(const std::vector<double>& v);

  inline int size() const {
    return rows() * cols();
  }

  DoubleMatrix smartReFlatten(int innerDup, int outerDup, int valsPerRow,
      int cols);

  static DoubleMatrix smartUnFlatten(const std::vector<double>& vals,
      int innerDup, int outerDup, int valsPerRow, int cols);

  static std::string niceFormat(double v);
  //static std::string niceFormat(cx_double v);

  /*DoubleMatrixArray getConvolution(const DoubleMatrixArray& filters,
      int strideRows, int strideCols) const;*/

  DoubleMatrix getConvolution(const DoubleMatrix& filter, int strideRows,
      int strideCols) const;

  DoubleMatrix getMeanAlongRows() const;
  DoubleMatrix getMeanAlongCols() const;
  DoubleMatrix getSumAlongRows() const;
  DoubleMatrix getSumAlongCols() const;

  void debugPrint(std::ostream&out, int verboseLevel) const;

  void appendRow(const std::vector<double>& vec);

  /** Appends given matrix to the right of this matrix */
  void appendCols(const DoubleMatrix&other);

  /** Appends given matrix to the bottom of this matrix */
  void appendRows(const DoubleMatrix &other);

  void clear();

};

std::ostream& operator<<(std::ostream&out, const DoubleMatrix&dm);

#endif /* DOUBLEMATRIX_H_ */
