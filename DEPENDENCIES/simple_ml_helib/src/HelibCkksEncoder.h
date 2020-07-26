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

#ifndef SRC_ML_HE_EXAMPLES_HELIBCKKSENCODER_H
#define SRC_ML_HE_EXAMPLES_HELIBCKKSENCODER_H

#include "AbstractEncoder.h"
#include "HelibCkksContext.h"

class HelibCkksEncoder : public AbstractEncoder {

	HelibCkksContext& he;

public:

	HelibCkksEncoder(HelibCkksContext& he);
  ~HelibCkksEncoder();

  // encode
  void encode(PTile& res, const std::vector<double>& vals, int chainIndex) const override;
  void encode(PTile& res, const std::vector<std::complex<double>>& vals, int chainIndex) const override;

  // decode
  std::vector<double> decodeDouble(const PTile& src) const override;
  std::vector<std::complex<double>> decodeComplex(const PTile& src) const override;

  // encrypt
  void encrypt(CTile& res, const PTile& src) const override;

  // decrypt
  void decrypt(PTile& res, const CTile& src) const override;

  // encode + encrypt
  void encodeEncrypt(CTile& res, const std::vector<double>& vals, int chainIndex) const override;
  void encodeEncrypt(CTile& res, const std::vector<std::complex<double>>& vals, int chainIndex) const override;

  // decrypt + decode
  std::vector<double> decryptDecodeDouble(const CTile& src) const override;
  std::vector<std::complex<double>> decryptDecodeComplex(const CTile& src) const override;
};

#endif /* SRC_ML_HE_EXAMPLES_HELIBCKKSENCODER_H */
