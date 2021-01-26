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

#ifndef SRC_HELAYERS_ABSTRACTENCODER_H
#define SRC_HELAYERS_ABSTRACTENCODER_H

#include "AbstractCiphertext.h"
#include "AbstractPlaintext.h"

namespace helayers {

///@brief Internal abstract implementation of an encoder, do not use directly.
/// Used as parent class for concrete classes implementing encoders in
/// various schemes.
/// Do not use directly, use a Encoder class instead.
class AbstractEncoder
{

  virtual double assertEqualsHelper(
      const std::string& title,
      const std::vector<std::complex<double>>& vals,
      const std::vector<std::complex<double>>& expectedVals,
      double eps,
      bool percent) const;

public:
  AbstractEncoder();
  ~AbstractEncoder();
  AbstractEncoder(const AbstractEncoder& src) = delete;
  AbstractEncoder& operator=(const AbstractEncoder& src) = delete;

  // manipulating the default scale
  virtual void setDefaultScale(double scale);
  virtual double getDefaultScale() const;
  virtual void restoreDefaultScale();

  // manipulating decrypt added noise policy
  virtual void setDecryptAddedNoiseEnabled(bool val);
  virtual bool getDecryptAddedNoiseEnabled() const;
  virtual void setDecryptAddedNoisePrecision(int val);
  virtual int getDecryptAddedNoisePrecision() const;

  // encode
  virtual void encode(AbstractPlaintext& res, int val, int chainIndex) const;
  virtual void encode(AbstractPlaintext& res, double val, int chainIndex) const;
  virtual void encode(AbstractPlaintext& res,
                      const std::vector<int>& vals,
                      int chainIndex) const;
  virtual void encode(AbstractPlaintext& res,
                      const std::vector<long>& vals,
                      int chainIndex) const;
  virtual void encode(AbstractPlaintext& res,
                      const std::vector<double>& vals,
                      int chainIndex) const = 0;
  virtual void encode(AbstractPlaintext& res,
                      const std::vector<std::complex<double>>& vals,
                      int chainIndex) const = 0;

  // decode
  virtual std::vector<int> decodeInt(const AbstractPlaintext& src) const;
  virtual std::vector<long> decodeLong(const AbstractPlaintext& src) const;
  virtual std::vector<double> decodeDouble(
      const AbstractPlaintext& src) const = 0;
  virtual std::vector<std::complex<double>> decodeComplex(
      const AbstractPlaintext& src) const = 0;

  // encrypt
  virtual void encrypt(AbstractCiphertext& res,
                       const AbstractPlaintext& src) const = 0;

  // decrypt
  virtual void decrypt(AbstractPlaintext& res,
                       const AbstractCiphertext& src) const = 0;

  // encode + encrypt
  virtual void encodeEncrypt(AbstractCiphertext& res,
                             const std::vector<int>& vals,
                             int chainIndex) const;
  virtual void encodeEncrypt(AbstractCiphertext& res,
                             const std::vector<long>& vals,
                             int chainIndex) const;
  virtual void encodeEncrypt(AbstractCiphertext& res,
                             const std::vector<double>& vals,
                             int chainIndex) const = 0;
  virtual void encodeEncrypt(AbstractCiphertext& res,
                             const std::vector<std::complex<double>>& vals,
                             int chainIndex) const = 0;

  // decrypt + decode
  virtual std::vector<int> decryptDecodeInt(
      const AbstractCiphertext& src) const;
  virtual std::vector<long> decryptDecodeLong(
      const AbstractCiphertext& src) const;
  virtual std::vector<double> decryptDecodeDouble(
      const AbstractCiphertext& src) const = 0;
  virtual std::vector<std::complex<double>> decryptDecodeComplex(
      const AbstractCiphertext& src) const = 0;

  virtual double assertEquals(const AbstractCiphertext& c,
                              const std::string& title,
                              const std::vector<int>& expectedVals,
                              double eps,
                              bool percent) const;
  virtual double assertEquals(const AbstractCiphertext& c,
                              const std::string& title,
                              const std::vector<long>& expectedVals,
                              double eps,
                              bool percent) const;
  virtual double assertEquals(const AbstractCiphertext& c,
                              const std::string& title,
                              const std::vector<double>& expectedVals,
                              double eps,
                              bool percent) const;
  virtual double assertEquals(
      const AbstractCiphertext& c,
      const std::string& title,
      const std::vector<std::complex<double>>& expectedVals,
      double eps,
      bool percent) const;
};
}

#endif /* SRC_HELAYERS_ABSTRACTENCODER_H */
