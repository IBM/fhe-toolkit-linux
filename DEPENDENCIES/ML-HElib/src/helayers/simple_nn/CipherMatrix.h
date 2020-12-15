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

#ifndef SRC_HELAYERS_CIPHERMATRIX_H
#define SRC_HELAYERS_CIPHERMATRIX_H

#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>
#include "helayers/hebase/hebase.h"

namespace helayers {

/// A class for holding a matrix of ciphertexts.
class CipherMatrix
{

  HeContext* he;

  boost::numeric::ublas::tensor<CTile> tiles;

  int numFilledSlots;

  friend class CipherMatrixEncoder;

public:
  /// Construct an empty object.
  /// @param[in] he the underlying context.
  CipherMatrix(HeContext& he);

  /// Default copy constructor.
  CipherMatrix(const CipherMatrix& src) = default;

  ~CipherMatrix();

  /// Default assignment operator.
  CipherMatrix& operator=(const CipherMatrix& src) = default;

  /// Save object to binary stream.
  /// @param[in] stream output stream to write to
  std::streamoff save(std::ostream& stream) const;

  /// Load object from binary stream.
  /// @param[in] stream output stream to read from
  std::streamoff load(std::istream& stream);

  /// Elementwise add other matrix.
  /// @param[in] other matrix to add to
  void add(const CipherMatrix& other);

  /// Returns a CipherMatrix containing the matrixmultiplication result
  /// @param[in] other matrix to multiply with
  CipherMatrix getMatrixMultiply(const CipherMatrix& other) const;

  /// Elementwise square.
  void square();

  /// Returns a copy of this matrix with elementwise square applied.
  CipherMatrix getSquare() const;

  /// Relinearize all ciphertexts.
  void relinearize();

  /// Rescale all ciphertexts.
  void rescale();

  /// Returns the current chain index of ciphertexts.
  int getChainIndex() const;
};
}

#endif /* SRC_HELAYERS_CIPHERMATRIX_H */
