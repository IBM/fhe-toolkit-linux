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

#ifndef SRC_HELAYERS_BITWISEEVALUATOR_H
#define SRC_HELAYERS_BITWISEEVALUATOR_H

#include "impl/AbstractBitwiseEvaluator.h"
#include "CTile.h"

namespace helayers {

/// Used to perform operations on bitwise CTiles.
/// Can only be constructed by bitwise HeContext.
class BitwiseEvaluator
{
  HeContext& h;
  std::shared_ptr<AbstractBitwiseEvaluator> impl;

public:
  /// Constructs a ready to use object.
  /// @param[in] he the underlying context.
  BitwiseEvaluator(HeContext& he);

  ~BitwiseEvaluator() {}

  /// Returns the MSB of c.
  /// The result is returned is one-bit, unsigned bitwise CTile.
  /// @param[in] c  The CTile to get its MSB.
  CTile getMSB(const CTile& c) const;

  /// Returns The NOT of the MSB of c.
  ///  /// The result is returned as one-bit, unsigned bitwise CTile.
  /// @param[in] c  The CTile to get its flipped MSB.
  CTile getFlippedMSB(const CTile& c) const;

  /// Sets c to be signed if val is true, and unsigned otherwise.
  /// Note that changing c from signed to unsigned (or converse) will cause its
  /// value to change accordingly. The value of a signed CTile is interpreted
  /// using 2's complement method, and the value of an unsigned CTile is
  /// interpreted as an ordinary
  /// binary number.
  /// @param[in] c    To set to signed/unsigned.
  /// @param[in] val  Whether to set c to signed or unsigned.
  void setIsSigned(CTile& c, bool val) const;

  /// Returns true if c is signed and false otherwise.
  /// @param[in] c To check whether it is signed.
  bool getIsSigned(const CTile& c) const;

  // CTile abs(const CTile &c) const;

  /// Returns a vector of the bits of "c".
  /// Each bit is returned as a one-bit, unsigned bitwise CTile.
  /// @param[in] c The CTile to split.
  std::vector<CTile> split(const CTile& c) const;

  /// Combines the given bitwise CTiles into one.
  /// Combines all the bitwise CTiles in cs[from] until cs[to] (inclusive)
  /// into one bitwise CTile containing the first "bitsPerElement" LSB bits
  /// of each ciphertext. The LSB of the result is the first bit of cs[from]. If
  /// "to" is -1, then all the ciphertexts in cs[from] until cs.end() are taken.
  /// @param[in] cs              The bitwise CTiles to combine
  /// @param[in] from            The index to start combining from.
  /// @param[in] out             The index to combine until.
  /// @param[in] bitsPerElement  How many bits to take from each CTile in "cs".
  CTile combine(const std::vector<CTile> cs,
                int from = 0,
                int to = -1,
                int bitsPerElement = 1) const;

  /// Checks whether c1 == c2.
  /// Returns (an encryption of) 1 if c1 == c2, and (an encryption of) 0
  /// otherwise.
  /// The result is returned is one-bit, unsigned, bitwise CTile.
  /// @param[in] c1,c2 To evaluate c1 == c2 on.
  CTile isEqual(const CTile& c1, const CTile& c2) const;

  /// Returns first "targetBits" LSBs of c1*c2.
  /// If one of c1 or c2 is signed, the returned CTile will be signed.
  /// Otherwise,
  /// the returned CTile will be unsigned. The scale of the result is set to the
  /// product of c1's and c2's scales.
  /// @param[in] c1,c2        To multiply.
  /// @param[in] targetBits   The number of the bits of the result. If -1, the
  ///                         result will contain
  ///                         getNumBits(c1) + getNumBits(c2) bits.
  CTile multiply(const CTile& c1, const CTile& c2, int targetBits = -1) const;

  /// Returns the first "targetBits" LSBs of c1 + c2.
  /// If one of c1 or c2 is signed, the returned CTile will be signed.
  /// Otherwise,
  /// the returned CTile will be unsigned.
  /// @param[in] c1,c2        To add
  /// @param[in] targetBits   The number of the bits of the result. If -1, the
  ///                         result will contain.
  ///                         max(getNumBits(c1) + getNumBits(c2) + 1) bits.
  /// @throw runtime_error If the scales of c1 and c2 are different.
  CTile add(const CTile& c1, const CTile& c2, int targetBits = -1) const;

  /// Returns the first "targetBits" LSBs of c1 - c2.
  /// If one of c1 or c2 is signed, the returned CTile will be signed.
  /// Otherwise,
  /// the returned CTile will be unsigned.
  /// @param[in] c1,c2        To subtract.
  /// @param[in] targetBits   The number of the bits of the result. If -1, the
  ///                         result will contain getNumBits(c1) bits.
  /// @throw runtime_error If the scales of c1 and c2 are different.
  CTile sub(const CTile& c1, const CTile& c2, int targetBits = -1) const;

  /// Returns the result of multiplying all bits of c by the given "bit".
  /// @param[in] c     To multiply its bits by the given bit.
  /// @param[in] bit   To multiply c by.
  CTile multiplyBit(const CTile& c, const CTile& bit) const;

  /// Bitwise xor between c1 and c2.
  /// I c1 is signed, the result will be signed. Otherwise, the result will be
  /// unsigned.
  /// @param[in] c1,c2 To perform bitwise xor between.
  CTile bitwiseXor(const CTile& c1, const CTile& c2) const;

  /// Returns the number of bits of c.
  /// @param[in] c To check its number of bits.
  int getNumBits(const CTile& c) const;

  /// Updates c to contain "bits" bits.
  /// If getNumBits(c) > bits, then MSBs of c are removed until it contains only
  /// "bits" bits.
  /// If getNumBits(c) < bits and c is signed, then the MSB of c is added to c
  /// until it contains "bits" bits.
  /// If getNumBits(c) < bits and c is unsigned, then 0 is added to c until it
  /// contains "bits" bits.
  /// @param[in, out] c    The CTile to update to contain "bits" bit.
  /// @param[in] bits      The number of bits c will contain.
  void setNumBits(CTile& c, int bits) const;

  /// Returns the default number of bits to a CTile.
  /// This default number is taken from the HeContext this BitwiseEvaluator was
  /// initialized with.
  int getDefaultNumBits() const;

  /// Prints debug info.
  /// @param[in] c                The CTile to print debug info about.
  /// @param[in] title            The title used to display the debug message.
  /// @param[in] maxElements      Maximal number of slots to debug-print.
  /// @param[in] verbose          Verbose used while printing.
  /// @param[in,out] out          The ostream to print to.
  void debugPrintWithBinary(const CTile& c,
                            const std::string& title,
                            int maxElements = 4,
                            int verbose = 0,
                            std::ostream& out = std::cout) const;

  /// Returns the scale of c.
  /// @param[in] c To get its scale.
  double getScale(const CTile& c) const;

  /// Returns max(c1, c2).
  /// @param[in] c1,c2 To compute their maximum.
  CTile max(const CTile& c1, const CTile& c2) const;

  /// Returns min(c1, c2).
  /// @param[in] c1,c2 To compute their maximum.
  CTile min(const CTile& c1, const CTile& c2) const;

  /// Checks whether c1 < c2.
  /// Returns (an encryption of) 1 if c1 < c2, and (an encryption of) 0
  /// otherwise.
  /// The result is returned is one-bit, unsigned, bitwise CTile.
  /// @param[in] c1,c2 To evaluate c1 < c2 on.
  CTile isGreater(const CTile& c1, const CTile& c2) const;

  /// Checks whether c1 > c2.
  /// Returns (an encryption of) 1 if c1 > c2, and (an encryption of) 0
  /// otherwise.
  /// The result is returned is one-bit, unsigned, bitwise CTile.
  /// @param[in] c1,c2 To evaluate c1 > c2 on.
  CTile isLess(const CTile& c1, const CTile& c2) const;

  /// Checks whether c1 >= c2.
  /// Returns (an encryption of) 1 if c1 >= c2, and (an encryption of) 0
  /// otherwise.
  /// The result is returned is one-bit, unsigned, bitwise CTile.
  /// @param[in] c1,c2 To evaluate c1 >= c2 on.
  CTile isGreaterEqual(const CTile& c1, const CTile& c2) const;

  /// Checks whether c1 <= c2.
  /// Returns (an encryption of) 1 if c1 <= c2, and (an encryption of) 0
  /// otherwise.
  /// The result is returned is one-bit, unsigned, bitwise CTile.
  /// @param[in] c1,c2 To evaluate c1 <= c2 on.
  CTile isLessEqual(const CTile& c1, const CTile& c2) const;
};
}

#endif /* SRC_HELAYERS_BITWISEEVALUATOR_H */