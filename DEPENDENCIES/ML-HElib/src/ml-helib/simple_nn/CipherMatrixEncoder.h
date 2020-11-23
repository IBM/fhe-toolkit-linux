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

#ifndef SRC_HEDGE_CIPHERMATRIXENCODER_H
#define SRC_HEDGE_CIPHERMATRIXENCODER_H

#include "CipherMatrix.h"
#include "hedge/hebase/hebase.h"

namespace hedge {

/** A class for encoding, encrypting, decoding, and decrypting a CipherMatrix.
 *
 * Encrypting raw plain data in HE involves two stages: encode and encrypt.
 * Similary, the opposite direction involes decrypt and decode.
 *
 * This class performs both steps in each direction, encoding and encrypting a
 * plain matrix to a CipherMatrix, and vice versa.
 *
 * The plain values are represented using boost::numeric::ublas::tensor
 * with element values that are either double or complex.
*/
class CipherMatrixEncoder
{

  HeContext& he;

  Encoder enc;

public:
  CipherMatrixEncoder(HeContext& he);
  ~CipherMatrixEncoder();
  CipherMatrixEncoder(const CipherMatrixEncoder& src) = delete;
  CipherMatrixEncoder& operator=(const CipherMatrixEncoder& src) = delete;

  // encode + encrypt
  void encodeEncrypt(CipherMatrix& res,
                     const boost::numeric::ublas::tensor<double>& vals,
                     int chainIndex = -1) const;
  void encodeEncrypt(
      CipherMatrix& res,
      const boost::numeric::ublas::tensor<std::complex<double>>& vals,
      int chainIndex = -1) const;

  // decrypt + decode
  boost::numeric::ublas::tensor<double> decryptDecodeDouble(
      const CipherMatrix& src) const;
  boost::numeric::ublas::tensor<std::complex<double>> decryptDecodeComplex(
      const CipherMatrix& src) const;
};
}

#endif /* SRC_HEDGE_CIPHERMATRIXENCODER_H */
