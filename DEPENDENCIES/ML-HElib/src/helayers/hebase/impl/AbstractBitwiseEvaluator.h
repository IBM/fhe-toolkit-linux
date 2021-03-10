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

#ifndef ABSTRACTBITWISEEVALUATOR_H
#define ABSTRACTBITWISEEVALUATOR_H

#include "AbstractCiphertext.h"
#include <iostream>
#include <vector>

namespace helayers {

///@brief For internal use.
class AbstractBitwiseEvaluator
{

public:
  AbstractBitwiseEvaluator() {}
  virtual ~AbstractBitwiseEvaluator() {}

  // Returns the sign bit of c (as an unsigned bitwise ciphertext, containing
  // one bit).
  virtual std::shared_ptr<AbstractCiphertext> getMSB(
      const AbstractCiphertext& c) const = 0;

  // Returns a ciophertext containing "1" if the MSB of "c" is 0, and a
  // ciphertext containing "0" otherwise.
  virtual std::shared_ptr<AbstractCiphertext> getFlippedMSB(
      const AbstractCiphertext& c) const = 0;

  virtual std::shared_ptr<AbstractCiphertext> hamming(
      const AbstractCiphertext& c,
      int from = 0,
      int to = -1) const = 0;

  // Updates "c" to treat its stored binary value as a value in 2s complement
  // representation. Note that calling this function might change the value of
  // c.
  virtual void setIsSigned(AbstractCiphertext& c, bool val) const = 0;

  // Updates "c" to treat its stored binary value as unsigned binary number.
  // Note that calling this function might change the value of c.
  virtual bool getIsSigned(const AbstractCiphertext& c) const = 0;

  // Returns the absolute value of c.
  // AbstractCiphertext abs(const AbstractCiphertext &c) const;

  // Returns a vector of the bits of "c", each bit as a separate unsigned
  // bitwise ciphertext, containing one bit.
  virtual std::vector<std::shared_ptr<AbstractCiphertext>> split(
      const AbstractCiphertext& c) const = 0;

  // Combines all the bitwise ciphertexts in cs[from] until cs[to] (inclusive)
  // into one bitwise ciphertext containing the first "bitsPerElement" LSB bits
  // of each ciphertext. The LSB of the result is the first bit of cs[from]. If
  // "to" is -1, then all the ciphertexts in cs[from] until cs.end() are taken.
  virtual std::shared_ptr<AbstractCiphertext> combine(
      const std::vector<std::shared_ptr<AbstractCiphertext>>& cs,
      int from = 0,
      int to = -1,
      int bitsPerElement = 1) const = 0;

  // Checks whether "c1" and "c2" are equal.
  // The result is returned as a 2 bit ciphertext which is "01" if the bits are
  // equal and
  //"00" otherwise.
  virtual std::shared_ptr<AbstractCiphertext> isEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;

  // Returns the result of c1*c2. If "targetBits" is not -1, the return result
  // will contain the "targetBits" LSBs of the actual multiplication result.
  // Otherwise, the result will contain getNumBits(c1) + getNumBits(c2) bits.
  // The scale of the result will be set to the product of the scales of "c1"
  // and "c2".
  virtual std::shared_ptr<AbstractCiphertext> multiply(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2,
      int targetBits = -1) const = 0;

  // Returns the result of c1+c2. If "targetBits" is not -1 the return result
  // will contain the "targetBits" LSBs of the actual addition result.
  // Otherwise, the result will contain getNumBits(c1) + 1 bits. This function
  // assumes both "c1" and "c2" have the same scale. The scale of the result
  // will be set to the scale of the received ciphertexts.
  virtual std::shared_ptr<AbstractCiphertext> add(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2,
      int targetBits = -1) const = 0;

  // Returns the result of c1-c2. If "targetBits" is not -1 the return result
  // will contain the "targetBits" LSBs of the actual subtraction result.
  // Otherwise, the result will contain getNumBits(c1) bits. This function
  // assumes both "c1" and "c2" have the same scale. The scale of the result
  // will be set to the scale of the received ciphertexts.
  virtual std::shared_ptr<AbstractCiphertext> sub(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2,
      int targetBits = -1) const = 0;

  virtual std::shared_ptr<AbstractCiphertext> multiplyBit(
      const AbstractCiphertext& c,
      const AbstractCiphertext& bit) const = 0;

  virtual std::shared_ptr<AbstractCiphertext> bitwiseXor(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;

  // Returns number of bits in c
  virtual int getNumBits(const AbstractCiphertext& c) const = 0;

  virtual void setNumBits(AbstractCiphertext& c, int bits) const = 0;

  // Returns the default number of bits to a newly constructed bitwise
  // ciphertext
  virtual int getDefaultNumBits() const = 0;

  virtual void debugPrintWithBinary(const AbstractCiphertext& c,
                                    const std::string& title,
                                    int maxElements = 4,
                                    int verbose = 0,
                                    std::ostream& out = std::cout) const = 0;

  virtual double getScale(const AbstractCiphertext& c) const = 0;

  virtual std::shared_ptr<AbstractCiphertext> max(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;
  virtual std::shared_ptr<AbstractCiphertext> min(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;

  virtual std::shared_ptr<AbstractCiphertext> isGreater(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;
  virtual std::shared_ptr<AbstractCiphertext> isLess(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;
  virtual std::shared_ptr<AbstractCiphertext> isGreaterEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;
  virtual std::shared_ptr<AbstractCiphertext> isLessEqual(
      const AbstractCiphertext& c1,
      const AbstractCiphertext& c2) const = 0;
};
} // namespace helayers

#endif