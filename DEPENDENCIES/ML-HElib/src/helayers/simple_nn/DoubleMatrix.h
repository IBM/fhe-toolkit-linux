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

#ifndef DOUBLEMATRIX_H_
#define DOUBLEMATRIX_H_

#include <vector>
#include <iostream>
#include <fstream>
#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>
#include <boost/numeric/ublas/matrix.hpp>
//#include "norms.h"

namespace helayers {

class DoubleMatrixArray;

using double_matrix_t = boost::numeric::ublas::matrix<double>;

/// A class for holding a matrix of doubles
class DoubleMatrix
{

  double_matrix_t m;

public:
  /// Create an empty matrix
  DoubleMatrix();

  /// Create a matrix with a given size and fill it with a value
  /// @param[in] r rows
  /// @param[in] c cols
  /// @param[in] fixedVal value to fill
  DoubleMatrix(int r, int c, double fixedVal = 0);

  /// Create a matrix loaded from text file
  /// @param[in] matrixFile text file name
  DoubleMatrix(std::ifstream& matrixFile);

  /// Create a matrix loaded from an h5 file
  /// @param[in] h5File h5 file name
  /// @param[in] path path inside file
  DoubleMatrix(const std::string& h5File, const std::string& path);

  /// Create a matrix given a flat vector of values,
  /// reshaped to a matrix with given number of rows.
  /// The flat vector is flattened in row-major order.
  /// @param[in] v flat vector of values.
  /// @param[in] matrixRows number of rows.
  DoubleMatrix(std::vector<double> v, int matrixRows);

  /// Create a matrix given a 2d array of float values.
  /// First dimension is rows.
  /// @param[in] matrix 2d array of values.
  DoubleMatrix(const std::vector<std::vector<float>>& matrix);

  /// Create a matrix given a 2d array of double values.
  /// First dimension is rows.
  /// @param[in] matrix 2d array of values.
  DoubleMatrix(const std::vector<std::vector<double>>& matrix);

  /// Create a matrix with given size, and copy to it given 2d array.
  /// The array is assumed to be smaller than matrix.
  /// @param[in] matrix 2d array of values to copy.
  /// @param[in] r number of rows to create.
  /// @param[in] c number of columns to create.
  /// @param[in] fixedVal initial values to fill the matrix with.
  DoubleMatrix(const std::vector<std::vector<double>>& matrix,
               int r,
               int c,
               double fixedVal = 0);

  DoubleMatrix(const boost::numeric::ublas::tensor<double>& t);

  virtual ~DoubleMatrix();

  /// Save to binary stream
  /// @param[in] stream binary stream
  std::streamoff save(std::ostream& stream) const;

  /// Load from binary stream
  /// @param[in] stream binary stream
  std::streamoff load(std::istream& stream);

  /// Load from an h5 file.
  /// @param[in] h5File file name
  /// @param[in] path path inside file
  void load(const std::string& h5File, const std::string& path);

  /// Returns the matrix values as a 2d array.
  std::vector<std::vector<double>> getVals() const;

  boost::numeric::ublas::tensor<double> getTensor() const;

  /// Returns the value at i,j.
  /// @param[in] i row index
  /// @param[in] j column index
  inline double get(int i, int j) const { return m(i, j); }

  /// Sets the value at i,j.
  /// @param[in] i row index
  /// @param[in] j column index
  /// @param[in] val value to set
  inline double set(int i, int j, double val) { return m(i, j) = val; }

  /// Returns number of rows.
  inline int rows() const { return m.size1(); }

  /// Returns number of columns.
  inline int cols() const { return m.size2(); }

  /// Subtracts other matrix elementwise.
  /// @param[in] other matrix to subtract
  /// @throw invalid_argument if dimensions mismatch
  DoubleMatrix& operator-=(const DoubleMatrix& other);

  /// Adds other matrix elementwise.
  /// @param[in] other matrix to add
  /// @throw invalid_argument if dimensions mismatch
  DoubleMatrix& operator+=(const DoubleMatrix& other);

  /// Adds a value at position i,j.
  /// @param[in] i row index
  /// @param[in] j column index
  /// @param[in] val value to add
  inline void addAt(double val, int i, int j) { m(i, j) += val; }

  // Adds a smaller matrix at position startRow,startCol
  /// @param[in] other matrix to add
  /// @param[in] startRow row index where other is placed
  /// @param[in] startCol column index where other is placed
  /// @throw invalid_argument if other matrix dimensions exceed this matrix
  void addAt(const DoubleMatrix& other, int startRow, int startCol);

  /// Returns a copy of the matrix transposed
  DoubleMatrix getTranspose() const;

  /// Returns a sub matrix (row1,col1)-(row2,col2), with exlusive end indices.
  /// @param[in] row1 First row index
  /// @param[in] col1 First column index
  /// @param[in] row2 Last row index (exclusive)
  /// @param[in] col2 Last column index (exclusive)
  DoubleMatrix getSubMatrix(int row1, int col1, int row2, int col2) const;

  /// Transpose this matrix in place
  void transpose();

  /// Asserts equality with another matrix up to given tolerance.
  /// Prints maximal difference in either case.
  /// @param[in] title Title to print along side maximal diff.
  /// @param[in] other matrix to compare with.
  /// @param[in] tolerance maximal tolerance
  /// @throw invalid_argument if other dimensions mismatch
  /// @throw runtime_error if matrices differ in values
  void testEqual(const std::string& title,
                 const DoubleMatrix& other,
                 double tolerance = 1e-6) const;

  /// Asserts dimension equality with another matrix.
  /// @param[in] title Title to print in case of error
  /// @param[in] other matrix to compare with
  /// @throw invalid_argument if other dimensions mismatch
  void testSameSize(const std::string& title, const DoubleMatrix& other) const;

  /// Returns a boolean specifying whether all the elements in this DoubleMatrix
  /// are equal to the
  /// elements of "other", up to difference which is less than "tolerance"
  /// @param[in] other matrix to compare with
  /// @param[in] tolerance maximal allowed diff
  bool checkIfEqual(const DoubleMatrix& other, double tolerance = 1e-6) const;

  /// Returns a boolean specifying whether this DoubleMatrix has the same
  /// dimensions as the dimensions of "other".
  /// @param[in] other matrix to compare with
  bool checkIfSameSize(const DoubleMatrix& other) const;

  /// Verifies other has same size, and copies its values
  /// @param[in] other matrix to copy from
  void copy(const DoubleMatrix& other);

  /// Initializes a matrix based on given 2d array of floats.
  /// Overwrites any existing state.
  /// @param[in] matrix 2d array defining the matrix
  void init(const std::vector<std::vector<float>> matrix);

  /// Initializes a matrix based on given 2d array of doubles.
  /// Overwrites any existing state.
  /// @param[in] matrix 2d array defining the matrix.
  void init(const std::vector<std::vector<double>> matrix);

  /// Fills the matrix with random values.
  /// Keeps current dimensions.
  void initRandom();

  /// Fills the matrix with a constant value.
  /// Keeps current dimensions.
  /// @param[in] v value to fill
  void initConst(double v);

  /// Returns the maximal absolute diff with other matrix.
  /// @param[in] other matrix to compare with
  double getMaxDiff(const DoubleMatrix& other) const;

  /// Returns maximal absolute value.
  double getMaxAbs() const;

  /// Computes the maximal relative difference with other.
  /// For each pair of compared values v1 and v2:
  /// if max(abs(v1),abs(v2))<10*tolerance, then the diff is abs(v1-v2)
  /// otherwise, abs(v1-v2)/max(abs(v1),abs(v2))
  /// @param[in] other matrix to compare with
  /// @param[in] tolerance controls when to measure relative diff
  double getMaxRelDiff(const DoubleMatrix& other, double tolerance) const;

  /// Fills given vector with matrix values in row major order.
  /// @param[out] vals resulting values.
  void flattenIntoVec(std::vector<double>& vals) const;

  /// Returns a vector filled with matrix values in row major order.
  std::vector<double> getFlatten() const;

  /// Returns the result of matrix-multiplication between this and other.
  /// @param[in] other matrix to multiply with
  DoubleMatrix getMultiply(const DoubleMatrix& other) const;

  /// Computes matrix-multiplication between this and other.
  /// Result is stored in place.
  /// @param[in] other matrix to multiply with
  void multiply(const DoubleMatrix& other);

  /// Computes elementwise multiplication between this and other.
  /// Result is stored in place.
  /// @param[in] other matrix to multiply with
  void elementMultiply(const DoubleMatrix& other);

  /// Computes elementwise multiplication between this and other at a specified
  /// window.
  /// Result is stored in place.
  /// The other matrix is placed and multiplied against a specified location in
  /// this matrix,
  /// and is expected not to extend beyond this matrix edges.
  /// @param[in] other matrix to multiply with
  /// @param[in] startRow row index in this matrix to start multipling
  /// @param[in] startCol column index in this matrix to start multipling
  void elementMultiplyAt(const DoubleMatrix& other, int startRow, int startCol);

  /// Scalar multiplication.
  /// @param[in] scalar scalar to multiply with
  void multiplyByScalar(double scalar);

  /// Reduces the rows to a single row by calculating the mean of each column
  void meanAlongRows();

  /// Reduces the columns to a single column by calculating the mean of each row
  void meanAlongCols();

  /// Reduces the rows to a single row by calculating the sum of each column
  void sumAlongRows();

  /// Reduces the columns to a single column by calculating the sum of each row
  void sumAlongCols();

  /// Returns a copy of the matrix with each element duplicated along rows and
  /// columns.
  /// @param[in] dupRows how many times to duplicate along rows
  /// @param[in] dupCols how many times to duplicate along cols
  DoubleMatrix getDuplicate(int dupRows, int dupCols) const;

  /// Overwrites existing matrix with values from flat vector v in row major
  /// order.
  /// Dimensions remain the same.
  ///  @param[in] v vector of values to read from
  void unFlatten(const std::vector<double>& v);

  /// Returns number of elements in matrix
  inline int size() const { return rows() * cols(); }

  /// Returns a nice string representation of given number
  /// @param[in] v the number
  static std::string niceFormat(double v);

  /// Returns the convolution result between this matrix and given filter
  /// @param[in] filter A filter for convolution
  /// @param[in] strideRows Stride along row dimension
  /// @param[in] strideCols Stride along col dimension
  DoubleMatrix getConvolution(const DoubleMatrix& filter,
                              int strideRows,
                              int strideCols) const;

  /// Returns a copy of the matrix with the rows reduced to a single row by
  /// calculating the mean of each column.
  DoubleMatrix getMeanAlongRows() const;

  /// Returns a copy of the matrix with the columns reduced to a single column
  /// by
  /// calculating the mean of each row.
  DoubleMatrix getMeanAlongCols() const;

  /// Returns a copy of the matrix with the rows reduced to a single row by
  /// calculating the sum of each column.
  DoubleMatrix getSumAlongRows() const;

  /// Returns a copy of the matrix with the columns reduced to a single column
  /// by
  /// calculating the sum of each row.
  DoubleMatrix getSumAlongCols() const;

  /// Prints matrix.
  /// @param[in] out Stream to print to
  /// @param[in] verboseLevel Verbosity level
  void debugPrint(std::ostream& out, int verboseLevel) const;

  /// Appends a row to the matrix.
  /// @param[in] vec Values of row to add
  /// @throw invalid_argument If matrix is not empty and dimensions mismatch.
  void appendRow(const std::vector<double>& vec);

  /// Appends new columns to the matrix.
  /// @param[in] other Values to add
  /// @throw invalid_argument If matrix is not empty and dimensions mismatch.
  void appendCols(const DoubleMatrix& other);

  /// Appends new rows to the matrix.
  /// @param[in] other Values to add
  /// @throw invalid_argument If matrix is not empty and dimensions mismatch.
  void appendRows(const DoubleMatrix& other);

  /// Sets the matrix to a 0 by 0 empty matrix.
  void clear();

  /// Returns the index of the maximal value. Supported only for single row or
  /// single column matrices.
  /// @throw invalid_argument If matrix is empty or not a single row or single
  /// column.
  int argmax() const;
};

std::ostream& operator<<(std::ostream& out, const DoubleMatrix& dm);
} // namespace helayers

#endif /* DOUBLEMATRIX_H_ */
