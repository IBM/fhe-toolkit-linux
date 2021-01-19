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

#ifndef SRC_HELAYERS_HELIBBGVENCODER_H
#define SRC_HELAYERS_HELIBBGVENCODER_H

#include "helayers/hebase/impl/AbstractEncoder.h"
#include "HelibBgvContext.h"

namespace helayers {

class HelibBgvEncoder : public AbstractEncoder
{

  HelibBgvContext& he;

public:
  HelibBgvEncoder(HelibBgvContext& he);
  ~HelibBgvEncoder();

  // encode
  void encode(AbstractPlaintext& res,
              const std::vector<int>& vals,
              int chainIndex) const override;
  void encode(AbstractPlaintext& res,
              const std::vector<long>& vals,
              int chainIndex) const override;
  void encode(AbstractPlaintext& res,
              const std::vector<double>& vals,
              int chainIndex) const override;
  void encode(AbstractPlaintext& res,
              const std::vector<std::complex<double>>& vals,
              int chainIndex) const override;

  // decode
  std::vector<int> decodeInt(const AbstractPlaintext& src) const override;
  std::vector<long> decodeLong(const AbstractPlaintext& src) const override;
  std::vector<double> decodeDouble(const AbstractPlaintext& src) const override;
  std::vector<std::complex<double>> decodeComplex(
      const AbstractPlaintext& src) const override;

  // encrypt
  void encrypt(AbstractCiphertext& res,
               const AbstractPlaintext& src) const override;

  // decrypt
  void decrypt(AbstractPlaintext& res,
               const AbstractCiphertext& src) const override;

  // encode + encrypt
  void encodeEncrypt(AbstractCiphertext& res,
                     const std::vector<int>& vals,
                     int chainIndex) const override;
  void encodeEncrypt(AbstractCiphertext& res,
                     const std::vector<long>& vals,
                     int chainIndex) const override;
  void encodeEncrypt(AbstractCiphertext& res,
                     const std::vector<double>& vals,
                     int chainIndex) const override;
  void encodeEncrypt(AbstractCiphertext& res,
                     const std::vector<std::complex<double>>& vals,
                     int chainIndex) const override;

  // decrypt + decode
  std::vector<int> decryptDecodeInt(
      const AbstractCiphertext& src) const override;
  std::vector<long> decryptDecodeLong(
      const AbstractCiphertext& src) const override;
  std::vector<double> decryptDecodeDouble(
      const AbstractCiphertext& src) const override;
  std::vector<std::complex<double>> decryptDecodeComplex(
      const AbstractCiphertext& src) const override;
};
}

#endif /* SRC_HELAYERS_HELIBBGVENCODER_H */
