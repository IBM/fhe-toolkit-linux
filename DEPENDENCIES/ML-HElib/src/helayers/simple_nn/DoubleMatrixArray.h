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

#ifndef STRAINER_DOUBLEMATRIXARRAY_H_
#define STRAINER_DOUBLEMATRIXARRAY_H_

#include <fstream>
#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>
#include "DoubleMatrix.h"

namespace helayers {

///@brief A class for holding an array of matrices.
/// It used for handling a batch of matrices, and can
/// perform SIMD (single instruction multiple data) ops on all of them.
/// For example, matrix multiplication with another DoubleMatrixArray
/// means multiplying the corresponding i'th matrix in each array.
/// It can also be described as a 3d tensor.
class DoubleMatrixArray
{
  std::vector<DoubleMatrix> mats;

public:
  ///@brief Construct an empty DoubleMatrixArray object
  DoubleMatrixArray();

  ///@brief Construct DoubleMatrixArray of given size, filled with zeroes.
  ///
  ///@param rows Number of rows
  ///@param cols Number of columns
  ///@param len  Length of third dimension
  DoubleMatrixArray(int rows, int cols, int len);

  ///@brief Construct a new DoubleMatrixArray, initialized from text stream
  ///
  /// The file is expected to have the following format:
  /// [[mat_{1,1,1}, . . . , mat_{1,n,1}]
  /// [mat_{2,1,1}, . . . , mat_{2,n,1}]
  /// ...
  /// [mat_{m,1,1}, . . . , mat_{m,n,1}],
  /// ...
  /// [mat_{1,1,k}, . . . , mat_{1,n,k}]
  /// [mat_{2,1,k}, . . . , mat_{2,n,k}]
  /// ...
  /// [mat_{m,1,k}, . . . , mat_{m,n,k}]]
  ///@param matricesFile Stream to read from
  DoubleMatrixArray(std::ifstream& matricesFile);

  ///@brief Construct a new DoubleMatrixArray with a given matrix duplicated a
  /// number of times.
  ///
  ///@param len Size of the third dimension.
  ///@param fixedMatrix The matrix to be duplicated.
  DoubleMatrixArray(int len,
                    const std::vector<std::vector<double>>& fixedMatrix);

  ///@brief Construct a new DoubleMatrixArray with a given matrix duplicated a
  /// number of times.
  ///
  ///@param len Size of the third dimension.
  ///@param fixedMatrix The matrix to be duplicated.
  DoubleMatrixArray(int len,
                    const std::vector<std::vector<float>>& fixedMatrix);

  ///@brief Construct a new DoubleMatrixArray with a given matrix duplicated a
  /// number of times.
  ///
  ///@param len Size of the third dimension.
  ///@param fixedMatrix The matrix to be duplicated.
  DoubleMatrixArray(int len, const DoubleMatrix& fixedMatrix);

  ///@brief Construct a new DoubleMatrixArray from a given tensor.
  ///
  ///@param tensor A 3d tensor containing the values.
  DoubleMatrixArray(const boost::numeric::ublas::tensor<double>& tensor);

  virtual ~DoubleMatrixArray();

  ///@brief Initializes to a given size, filled with zeroes.
  /// Overwrites existing state.
  ///@param rows Number of rows
  ///@param cols Number of columns
  ///@param len  Length of third dimension
  void init(int rows, int cols, int len);

  ///@brief Initializes to a given matrix duplicated a
  /// number of times.
  /// Overwrites existing state.
  ///@param len Size of the third dimension.
  ///@param fixedMatrix The matrix to be duplicated.
  void init(int len, const std::vector<std::vector<double>>& fixedMatrix);

  ///@brief Initializes to a given matrix duplicated a
  /// number of times.
  /// Overwrites existing state.
  ///@param len Size of the third dimension.
  ///@param fixedMatrix The matrix to be duplicated.
  void init(int len, const std::vector<std::vector<float>>& fixedMatrix);

  ///@brief Initializes to a given matrix duplicated a
  /// number of times.
  /// Overwrites existing state.
  ///@param len Size of the third dimension.
  ///@param fixedMatrix The matrix to be duplicated.
  void init(int len, const DoubleMatrix& fixedMatrix);

  ///@brief Initializes to a given tensor.
  ///
  ///@param tensor A 3d tensor containing the values.
  void init(const boost::numeric::ublas::tensor<double>& tensor);

  ///@brief Initialize to values loaded from h5 file.
  ///
  /// Loads a single matrix from h5 file, and duplicates it along third
  /// dimension.
  ///
  ///@param len Number of duplicates along third dimension
  ///@param h5File h5 file to load from
  ///@param path path inside h5 file
  void initFromH5File(int len,
                      const std::string& h5File,
                      const std::string& path);

  ///@brief Save to binary stream.
  ///
  ///@param stream stream to save to.
  std::streamoff save(std::ostream& stream) const;

  ///@brief Load from binary stream.
  ///
  ///@param stream stream to load from
  std::streamoff load(std::istream& stream);

  ///@brief Loads a single matrix to a specific position along the third
  /// dimension.
  ///
  ///@param matrixIndex The position along the third dimension to load to
  ///@param h5File h5 file to load from
  ///@param path path inside h5 file
  void loadMatrix(int matrixIndex,
                  const std::string& h5File,
                  const std::string& path);

  ///@brief Returns the content of this object as a 3d tensor
  boost::numeric::ublas::tensor<double> getTensor() const;

  ///@brief Returns the result of convolution between this object and given
  /// filters.
  /// Filters are expected to have the same 3d dimension size as this object.
  /// The convolution is computed in SIMD fashion for each matrix and filter.
  ///@param filter Filter to compute convolution with.
  ///@param strideRows Convolution stride along rows
  ///@param strideCol Convolution stride along column
  DoubleMatrixArray getConvolution(const DoubleMatrixArray& filter,
                                   int strideRows,
                                   int strideCol) const;

  ///@brief Returns the size of third dimension
  inline int size() const { return mats.size(); }

  ///@brief Returns the i'th matrix, or i'th slice along third dimension
  ///
  ///@param i Index along third dimension
  inline const DoubleMatrix& getMat(int i) const { return mats[i]; }

  ///@brief Returns the i'th matrix, or i'th slice along third dimension
  ///
  ///@param i Index along third dimension
  inline DoubleMatrix& getMat(int i) { return mats[i]; }

  ///@brief Returns number of rows, or size of first dimension
  inline int rows() const { return size() == 0 ? 0 : getMat(0).rows(); }

  ///@brief Returns number of columns, or size of second dimension
  inline int cols() const { return size() == 0 ? 0 : getMat(0).cols(); }

  ///@brief Returns the result of multiplying this with other according to the
  /// rules of
  /// matrixmultiplication.
  /// Both arrays should have the same depth.
  /// Returns an object res such that
  /// res.getMat(i)=this.getMat(i)*other.getMat(i)
  ///@param other other object to multiply with
  DoubleMatrixArray getMatrixMultiply(const DoubleMatrixArray& other) const;

  ///@brief Returns a DoubleMatrixArray with every matrix transposed.
  DoubleMatrixArray getTranspose() const;

  // returns a slice along the rows/column dims.
  // end indices are exclusive

  ///@brief Returns a DoubleMatrixArray with every matrix sliced.
  ///
  /// @param row1 First row index
  /// @param col1 First column index
  /// @param row2 Last row index (exclusive)
  /// @param col2 Last column index (exclusive)
  DoubleMatrixArray getSubMatrix(int row1, int col1, int row2, int col2) const;

  ///@brief Returns a DoubleMatrixArray with a (1x1) slice of every matrix.
  ///
  /// @param row Row index
  /// @param col Column index
  DoubleMatrixArray getSubMatrix(int row, int col) const;

  ///@brief Returns a DoubleMatrixArray with a column slice of every matrix.
  ///@param col Column index
  DoubleMatrixArray getColumn(int col) const;

  ///@brief Returns a DoubleMatrixArray with the rows reduced to a single row by
  /// calculating the mean of each column
  DoubleMatrixArray getMeanAlongRows() const;

  ///@brief Returns a DoubleMatrixArray with the columns reduced to a single
  /// column by calculating the mean of each row
  DoubleMatrixArray getMeanAlongCols() const;

  ///@brief Returns a DoubleMatrixArray reduced to a single matrix containing
  /// the mean of all matrices
  DoubleMatrixArray getMeanInDepth() const;

  ///@brief Returns a DoubleMatrixArray with the rows reduced to a single row by
  /// calculating the sum of each column
  DoubleMatrixArray getSumAlongRows() const;

  ///@brief Returns a DoubleMatrixArray with the cols reduced to a single col by
  /// calculating the sum of each row
  DoubleMatrixArray getSumAlongCols() const;

  ///@brief Returns a DoubleMatrixArray reduced to a single matrix containing
  /// the sum of all matrices
  DoubleMatrixArray getSumInDepth() const;

  ///@brief Returns a DoubleMatrixArray resulting from multiplying this object
  /// with a scalar.
  ///@param scalar Scalar to multiyply with
  DoubleMatrixArray getMultiplyByScalar(double scalar) const;

  ///@brief Appends all elements to given vector.
  /// Dimension order: Cols, Rows, Depth (cols runs first)
  ///
  ///@param vec  Output vector
  void flattenIntoVec(std::vector<double>& vec) const;

  ///@brief Transposes each matrix in this double matrix array
  void transpose();

  ///@brief Elementwise square all elements
  void square();

  void add(const DoubleMatrixArray& other);

  // add a value to given position
  void addAt(double val, int row, int col);
  // add a smaller matrix other at position startRow,startCol
  void addAt(const DoubleMatrixArray& other, int startRow, int startCol);
  void sub(const DoubleMatrixArray& other);
  void elementMultiply(const DoubleMatrixArray& other);

  // multiplies elementwise the smaller matrix 'other' starting at
  // position startRow,startCol
  void elementMultiplyAt(const DoubleMatrixArray& other,
                         int startRow,
                         int startCol);
  void multiplyByScalar(double scalar);

  // sums across the depth dimension, and copies result to all elements in this
  // dimension
  void innerSum();

  // reduces the rows to a single row by calculating the mean of each column
  void meanAlongRows();
  // reduces the columns to a single column by calculating the mean of each row
  void meanAlongCols();
  // reduces the matrices to a single matrix containing the mean of all matrices
  void meanInDepth();
  // reduces the rows to a single row by calculating the sum of each column
  void sumAlongRows();
  // reduces the cols to a single col by calculating the sum of each row
  void sumAlongCols();
  // reduces the matrices to a signle matrix containing the sum of all matrices
  void sumInDepth();

  void initRandom();
  void initRandomSame();
  void initConst(double v);
  void initHotel();

  void debugPrint(std::ostream& out,
                  const std::string& title,
                  int verboseLevel = 1) const;

  // Makes sure that all elements in this DoubleMatrixArray are equal to the
  // elements of "other",
  // up to difference which is less than "eps".
  // This function fails in case the matrix arrays are not equal.
  double testEquals(const std::string& title,
                    const DoubleMatrixArray& other,
                    double eps = 1e-6) const;

  // tests that all elements within depth of supplied zs equal zero up to
  // given epsilon
  double testEqualsZero(const std::string& title,
                        const std::vector<int>& zs,
                        double eps = 1e-6) const;

  /** Tests matrices in all depths are the same */
  void testSameAlongDepth(const std::string& title, double eps = 1e-6) const;

  // Makes sure that the number of matrices in this "DoubleMatrixArray" is the
  // same as the number
  // of matrices in "other".
  // This function fails in case the sizes are different
  void testSameSize(const std::string& title,
                    const DoubleMatrixArray& other) const;

  // Makes sure that all double matrices have the same dimensions as the double
  // matrices in "other".
  // This function fails in case the matrices of the two objects have different
  // dimentions.
  void testSameDims(const std::string& title,
                    const DoubleMatrixArray& other) const;

  // Returns a boolean specifying whether the elements of this DoubleMatrixArray
  // are equal to
  // the elements of "other", up to difference which is less than "eps"
  bool checkIfEqual(const DoubleMatrixArray& other, double eps = 1e-6) const;

  // Returns a boolean specifying whether the number of matrices in this
  // "DoubleMatrixArray" is the
  // same as the number of matrices in "other".
  bool checkIfSameSize(const DoubleMatrixArray& other) const;

  // Returns a boolean specifying whether all double matrices have the same
  // dimensions as the double
  // matrices in "other"
  bool checkIfSameDims(const DoubleMatrixArray& other) const;

  void pushBackMatrix(const DoubleMatrix& mat);

  /** Returns a slice along the depth dimension.
   *  In the range [start,end)
   */
  DoubleMatrixArray depthSlice(int start, int end) const;

  /** Appends given matrix to the right of this matrix */
  void appendCols(const DoubleMatrixArray& other);

  /** Appends given matrix to the bottom of this matrix */
  void appendRows(const DoubleMatrixArray& other);

  void clear();

  /** Runs getMat(k).set(i,j,val) for all k */
  void setInAllDepth(int i, int j, double val);

  /** Returns the values at (i,j,k) for all k, scaled by scale */
  std::vector<double> getInAllDepth(int i, int j, double scale = 1.0) const;
};

std::ostream& operator<<(std::ostream&, const DoubleMatrixArray& dma);
}

#endif /* STRAINER_DOUBLEMATRIXARRAY_H_ */
