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

#ifndef SRC_HEDGE_ENCODER_H
#define SRC_HEDGE_ENCODER_H

#include <vector>
#include "HeContext.h"
#include "impl/AbstractEncoder.h"
#include "CTile.h"
#include "PTile.h"

namespace hedge {

class Encoder
{

  HeContext& he;

  std::shared_ptr<AbstractEncoder> impl;

  template <typename T>
  std::vector<T> verifyEncodeVecSize(const std::vector<T>& vec) const;

public:
  Encoder(HeContext& he);
  ~Encoder();
  Encoder(const Encoder& src) = delete;
  Encoder& operator=(const Encoder& src) = delete;

  void encode(PTile& res, int val, int chainIndex = -1) const;
  void encode(PTile& res, double val, int chainIndex = -1) const;

  /// Encode the given vector into a PTile. \n
  /// if the size of the given vector is less than he.slotCount(),
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown. \n
  /// @param[out] res The encoded PTile
  /// @param[in] vals The vector to encode
  /// @param[in] If HeContext is a CKKS context, specifies the chainIndex of
  ///            the resulting PTile. otherwise, this parameter is ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encode(PTile& res,
              const std::vector<int>& vals,
              int chainIndex = -1) const;
  void encode(PTile& res,
              const std::vector<long>& vals,
              int chainIndex = -1) const;
  void encode(PTile& res,
              const std::vector<double>& vals,
              int chainIndex = -1) const;
  void encode(PTile& res,
              const std::vector<std::complex<double>>& vals,
              int chainIndex = -1) const;

  // decode
  std::vector<int> decodeInt(const PTile& src) const;
  std::vector<long> decodeLong(const PTile& src) const;
  std::vector<double> decodeDouble(const PTile& src) const;
  std::vector<std::complex<double>> decodeComplex(const PTile& src) const;

  // encrypt
  void encrypt(CTile& res, const PTile& src) const;

  // decrypt
  void decrypt(PTile& res, const CTile& src) const;

  /// Encode and then encrypt the given vector into a CTile. \n
  /// if the size of the given vector is less than he.slotCount(),
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown. \n
  /// @param[out] res The encoded PTile
  /// @param[in] vals The vector to encode
  /// @param[in] If HeContext is a CKKS context, specifies the chainIndex of
  ///            the resulting CTile. otherwise, this parameter is ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encodeEncrypt(CTile& res,
                     const std::vector<int>& vals,
                     int chainIndex = -1) const;
  void encodeEncrypt(CTile& res,
                     const std::vector<long>& vals,
                     int chainIndex = -1) const;
  void encodeEncrypt(CTile& res,
                     const std::vector<double>& vals,
                     int chainIndex = -1) const;
  void encodeEncrypt(CTile& res,
                     const std::vector<std::complex<double>>& vals,
                     int chainIndex = -1) const;

  // decrypt + decode
  std::vector<int> decryptDecodeInt(const CTile& src) const;
  std::vector<long> decryptDecodeLong(const CTile& src) const;
  std::vector<double> decryptDecodeDouble(const CTile& src) const;
  std::vector<std::complex<double>> decryptDecodeComplex(
      const CTile& src) const;

  // print error statistics
  void printErrorStats(CTile& actualC,
                       std::vector<double> expectedZ,
                       std::ostream& out = std::cout,
                       bool asCsv = false) const;

  // check whether a chain index is valid
  int validateChainIndex(int chainIndex) const;

  // tests that all elements within expectedVals are equal to the equivalent
  // elements in c,
  // up to a given epsilon, and returns the max difference found
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<int>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<long>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<double>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<std::complex<double>>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;
};
}

#endif /* SRC_HEDGE_ENCODER_H */
