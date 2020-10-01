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

/// A class used to encode, encrypt, decode and decrypt ciphertexts and
/// plaintexts.
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

  /// Encodes the given integer into a PTile.
  /// If the size of the given vector is less than he.slotCount(),
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encode(PTile& res,
              const std::vector<int>& vals,
              int chainIndex = -1) const;

  /// Encodes the given double into a PTile.
  /// If the size of the given vector is less than he.slotCount(),
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encode(PTile& res,
              const std::vector<long>& vals,
              int chainIndex = -1) const;

  /// Encodes the given vector into a PTile.
  /// If the size of the given vector is less than he.slotCount(),
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encode(PTile& res,
              const std::vector<double>& vals,
              int chainIndex = -1) const;

  /// Encodes the given vector into a PTile.
  /// If the size of the given vector is less than he.slotCount(),
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting PTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encode(PTile& res,
              const std::vector<std::complex<double>>& vals,
              int chainIndex = -1) const;

  /// Decodes the value of the given PTile into a vector of ints.
  /// If the underlying FHE scheme is a scheme that supports floating point
  /// values, then "src" is first decrypted into a vector of doubles and then
  /// rounded to the nearest integers.
  /// @param[in] src The PTile to decode.
  std::vector<int> decodeInt(const PTile& src) const;

  /// Decodes the value of the given PTile into a vector of longs.
  /// If the underlying FHE scheme is a scheme that supports floating point
  /// values, then "src" is first decrypted into a vector of doubles and then
  /// rounded to the nearest integers.
  /// @param[in] src The PTile to decode.
  std::vector<long> decodeLong(const PTile& src) const;

  /// Decodes the value of the given PTile into a vector of doubles.
  /// @param[in] src The PTile to decode.
  std::vector<double> decodeDouble(const PTile& src) const;

  /// Decodes the value of the given PTile into a vector of complex<double>.
  /// @param[in] src The PTile to decode.
  std::vector<std::complex<double>> decodeComplex(const PTile& src) const;

  /// Encrypts "src" into "res".
  /// @param[out] res  The resulting CTile.
  /// @param[in]  src  The PTile to encrypt
  void encrypt(CTile& res, const PTile& src) const;

  /// Encrypts "src" into "res".
  /// @param[out] res  The resulting PTile.
  /// @param[in]  src  The CTile to encrypt
  void decrypt(PTile& res, const CTile& src) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than he.slotCount(),
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encodeEncrypt(CTile& res,
                     const std::vector<int>& vals,
                     int chainIndex = -1) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than he.slotCount(),
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a untime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encodeEncrypt(CTile& res,
                     const std::vector<long>& vals,
                     int chainIndex = -1) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than he.slotCount(),
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a runtime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > he.slotCount()
  void encodeEncrypt(CTile& res,
                     const std::vector<double>& vals,
                     int chainIndex = -1) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than he.slotCount(),
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than he.slotCount(), a runtime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error If vals.size() > he.slotCount()
  /// @throw runtime_error If the underlying FHE scheme is a scheme that does
  ///                      not support complex values (such as BGV, for
  ///                      example).
  void encodeEncrypt(CTile& res,
                     const std::vector<std::complex<double>>& vals,
                     int chainIndex = -1) const;

  /// Decodes and then decryots the given CTile into a vector of ints.
  /// If the underlying FHE scheme is a scheme that supports floating point
  /// values, then "src" is first decrypted into a vector of doubles and then
  /// rounded to the nearest integers.
  /// @param[in] src         The CTile to decode and decrypt.
  std::vector<int> decryptDecodeInt(const CTile& src) const;

  /// Decodes and then decryots the given CTile into a vector of ints.
  /// If the underlying FHE scheme is a scheme that supports floating point
  /// values, then "src" is first decrypted into a vector of doubles and then
  /// rounded to the nearest integers.
  /// @param[in] src         The CTile to decode and decrypt.
  std::vector<long> decryptDecodeLong(const CTile& src) const;

  /// Decodes and then decryots the given CTile into a vector of doubles.
  /// @param[in] src         The CTile to decode and decrypt.
  std::vector<double> decryptDecodeDouble(const CTile& src) const;

  /// Decodes and then decryots the given CTile into a vector of
  /// complex<double>.
  /// @param[in] src         The CTile to decode and decrypt.
  std::vector<std::complex<double>> decryptDecodeComplex(
      const CTile& src) const;

  /// Prints statstical information relating to the difference between "actualC"
  /// and "expectedZ".
  /// @param[in] actualC     The CTile to decode-decrypt and to compare to
  ///                        expectedZ.
  /// @param[in] expectedZ   The vector of doubles to compare actualC with.
  /// @param[in] out         The ostream to print to.
  ///
  void printErrorStats(CTile& actualC,
                       std::vector<double> expectedZ,
                       std::ostream& out = std::cout,
                       bool asCsv = false) const;

  /// checks whether a chain index is valid.
  /// @param[in] chainIndex To check its validity.
  int validateChainIndex(int chainIndex) const;

  /// Asserts that "c" and "expectedVals" are considered equal.
  /// "c" and "expectedVals" will be considered equal if when decrypting and
  /// decoding "c" into a vector, v, one of the following holds:
  /// 1. percent == false and fabs(v[i] - expectedVals[i]) < eps, foreach i.
  /// 2. percent == true and fabs(v[i] - expectedVals[i]) / fabs(v[i]) < eps,
  ///    foreach i.
  /// The maximum found absolute or relative difference (depending on the value
  /// of "percent" flag) is returned.
  /// @param[in] c                The CTile to compare with expectedVals
  /// @param[in] title            The title used in case of printing a failure
  ///                             message.
  /// @param[in] expectedVals     The vector to compare with "c".
  /// @param[in] eps              Comparison tolerance
  /// @param]in] percent          If true, the relative difference between "c"
  ///                             and "expectedVals" is checked. Otherwise,
  ///                             the absolute difference is checked.
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<int>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;

  /// Asserts that "c" and "expectedVals" are considered equal.
  /// "c" and "expectedVals" will be considered equal if when decrypting and
  /// decoding "c" into a vector, v, one of the following holds:
  /// 1. percent == false and fabs(v[i] - expectedVals[i]) < eps, foreach i.
  /// 2. percent == true and fabs(v[i] - expectedVals[i]) / fabs(v[i]) < eps,
  ///    foreach i.
  /// The maximum found absolute or relative difference (depending on the value
  /// of "percent" flag) is returned.
  /// @param[in] c                The CTile to compare with expectedVals
  /// @param[in] title            The title used in case of printing a failure
  ///                             message.
  /// @param[in] expectedVals     The vector to compare with "c".
  /// @param[in] eps              Comparison tolerance
  /// @param]in] percent          If true, the relative difference between "c"
  ///                             and "expectedVals" is checked. Otherwise,
  ///                             the absolute difference is checked.
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<long>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;

  /// Asserts that "c" and "expectedVals" are considered equal.
  /// "c" and "expectedVals" will be considered equal if when decrypting and
  /// decoding "c" into a vector, v, one of the following holds:
  /// 1. percent == false and fabs(v[i] - expectedVals[i]) < eps, foreach i.
  /// 2. percent == true and fabs(v[i] - expectedVals[i]) / fabs(v[i]) < eps,
  ///    foreach i.
  /// The maximum found absolute or relative difference (depending on the value
  /// of "percent" flag) is returned.
  /// @param[in] c                The CTile to compare with expectedVals
  /// @param[in] title            The title used in case of printing a failure
  ///                             message.
  /// @param[in] expectedVals     The vector to compare with "c".
  /// @param[in] eps              Comparison tolerance
  /// @param]in] percent          If true, the relative difference between "c"
  ///                             and "expectedVals" is checked. Otherwise,
  ///                             the absolute difference is checked.
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<double>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;

  /// Asserts that "c" and "expectedVals" are considered equal.
  /// "c" and "expectedVals" will be considered equal if when decrypting and
  /// decoding "c" into a vector, v, one of the following holds:
  /// 1. percent == false and fabs(v[i] - expectedVals[i]) < eps, foreach i.
  /// 2. percent == true and fabs(v[i] - expectedVals[i]) / fabs(v[i]) < eps,
  ///    foreach i.
  /// The maximum found absolute or relative difference (depending on the value
  /// of "percent" flag) is returned.
  /// @param[in] c                The CTile to compare with expectedVals
  /// @param[in] title            The title used in case of printing a failure
  ///                             message.
  /// @param[in] expectedVals     The vector to compare with "c".
  /// @param[in] eps              Comparison tolerance
  /// @param]in] percent          If true, the relative difference between "c"
  ///                             and "expectedVals" is checked. Otherwise,
  ///                             the absolute difference is checked.
  double assertEquals(const CTile& c,
                      const std::string& title,
                      const std::vector<std::complex<double>>& expectedVals,
                      double eps = 1e-6,
                      bool percent = false) const;
};
}

#endif /* SRC_HEDGE_ENCODER_H */
