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

#ifndef SRC_HELAYERS_ENCODER_H
#define SRC_HELAYERS_ENCODER_H

#include <vector>
#include "HeContext.h"
#include "impl/AbstractEncoder.h"
#include "CTile.h"
#include "PTile.h"

namespace helayers {

/// A class used to encode, encrypt, decode and decrypt ciphertexts and
/// plaintexts.
class Encoder
{

  HeContext& he;

  std::shared_ptr<AbstractEncoder> impl;

  /// Checks if a vector have appropriate number of slots.
  template <typename T>
  bool checkEncodeVectorSize(const std::vector<T>& vec) const;

  /// Adjusts a vector to have appropriate number of slots.
  template <typename T>
  std::vector<T> adjustEncodeVectorSize(const std::vector<T>& vec) const;

public:
  /// Constructs a ready to use object.
  /// @param[in] he the underlying context.
  Encoder(HeContext& he);

  ~Encoder();

  /// Copy constructor deleted. Construct new objects when needed.
  Encoder(const Encoder& src) = delete;

  /// Assignment deleted. Construct new objects when needed.
  Encoder& operator=(const Encoder& src) = delete;

  /// Default scale is a parameter relevant to CKKS-like schemes.
  /// It is the scale used to encode values into plaintext/ciphertext.
  /// If left unset, the encoder will use the scheme's default.
  /// After setting, the encoder will use the given value.
  /// setDefaultScale sets the scale that the encoder will use henceforth.
  /// @param[in] scale The new default scale.
  void setDefaultScale(double scale);

  /// Returns the scale used by the encoder as set by setDefaultScale.
  /// If no default was previously set, return the default scale define by the
  /// HeContext object.
  double getDefaultScale() const;

  /// Restores the default scale to the default scale defined by the HeContext
  /// object.
  void restoreDefaultScale();

  /// Allows to determine whether to enable the decrypt added noise. The default
  /// behaviour for libraries that supports this feature is to have it enabled.
  /// Decrypt added noise is relevant to CKKS scheme for the purpose of ensuring
  /// security.
  /// reference: "On the Security of Homomorphic Encryption on Approximate
  ///             Numbers", Baiyu Li and Daniele Micciancio,
  ///             https://eprint.iacr.org/2020/1533.pdf
  /// @param[in] val A boolean indicating whether to enable the decrypt added
  /// noise.
  void setDecryptAddedNoiseEnabled(bool val);

  /// Returns a boolean indicating whether to enable the decrypt added noise.
  bool getDecryptAddedNoiseEnabled() const;

  /// Allows to set the level of precision to be preserved in the process of
  /// adding decrypt noise, if the decrypt added noise is enabled. If decrypt
  /// added noise is not enabled, this value is ignored.
  /// @param[in] val An integer indicating precision to be preserved.
  /// @throw runtime_error If the undelying library does not support decrypt
  /// added noise
  void setDecryptAddedNoisePrecision(int val);

  /// Returns an integer indicating precision to be preserved.
  /// @throw runtime_error If the undelying library does not support decrypt
  /// added noise
  int getDecryptAddedNoisePrecision() const;

  /// Encodes the given int into all slots of a PTile.
  /// This is usually more efficient than regular encoding.
  /// @param[out] res The encoded PTile
  /// @param[in] val The value to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  void encode(PTile& res, int val, int chainIndex = -1) const;

  /// Encodes the given double into all slots of a PTile.
  /// This is usually more efficient than regular encoding.
  /// @param[out] res The encoded PTile
  /// @param[in] val The value to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  void encode(PTile& res, double val, int chainIndex = -1) const;

  /// Encodes the given vector of integers into a PTile.
  /// If the size of the given vector is less than slot count,
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than slot count, a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  /// @throw runtime_error if vals.size() > slot count
  void encode(PTile& res,
              const std::vector<int>& vals,
              int chainIndex = -1) const;

  /// Encodes the given vector of longs into a PTile.
  /// If the size of the given vector is less than slot count,
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than slot count, a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  /// @throw runtime_error if vals.size() > slot count
  void encode(PTile& res,
              const std::vector<long>& vals,
              int chainIndex = -1) const;

  /// Encodes the given vector of doubles into a PTile.
  /// If the size of the given vector is less than slot count,
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than slot count, a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  /// chainIndex of
  ///                        the resulting PTile. otherwise, this parameter is
  ///                        ignored.
  /// @throw runtime_error if vals.size() > slot count
  void encode(PTile& res,
              const std::vector<double>& vals,
              int chainIndex = -1) const;

  /// Encodes the given vector of complex numbers into a PTile.
  /// If the size of the given vector is less than slot count,
  /// the empty slots of the PTile will be set to zero. If the size of the given
  /// vector is more than slot count, a untime_error will be thrown.
  /// @param[out] res The    encoded PTile
  /// @param[in] vals The    vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting PTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > slot count
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
  /// if the size of the given vector is less than slot count,
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than slot count, a untime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > slot count
  void encodeEncrypt(CTile& res,
                     const std::vector<int>& vals,
                     int chainIndex = -1) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than slot count,
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than slot count, a untime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > slot count
  void encodeEncrypt(CTile& res,
                     const std::vector<long>& vals,
                     int chainIndex = -1) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than slot count,
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than slot count, a runtime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error if vals.size() > slot count
  void encodeEncrypt(CTile& res,
                     const std::vector<double>& vals,
                     int chainIndex = -1) const;

  /// Encodes and then encrypts the given vector into a CTile.
  /// if the size of the given vector is less than slot count,
  /// the empty slots of the CTile will be set to zero. If the size of the given
  /// vector is more than slot count, a runtime_error will be thrown.
  /// @param[out] res        The resulting CTile
  /// @param[in] vals        The vector to encode
  /// @param[in] chainIndex  If HeContext is a CKKS context, specifies the
  ///                        chainIndex of the resulting CTile. otherwise, this
  ///                        parameter is ignored.
  /// @throw runtime_error If vals.size() > slot count
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

  /// Reserved for debugging and internal use.
  const AbstractEncoder& getImpl() const { return *impl; };
};
} // namespace helayers

#endif /* SRC_HELAYERS_ENCODER_H */
