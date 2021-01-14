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

#ifndef SRC_HELAYERS_ABSTRACTCIPHER_H_
#define SRC_HELAYERS_ABSTRACTCIPHER_H_

#include <vector>
#include <memory>
#include <iostream>
#include <complex>

#include "AbstractPlaintext.h"
#include "helayers/hebase/HeContext.h"

namespace helayers {

class AbstractCiphertext
{

  HeContext& he;

  virtual std::shared_ptr<AbstractCiphertext> doClone() const = 0;

protected:
  AbstractCiphertext(const AbstractCiphertext& src) = default;

public:
  AbstractCiphertext(HeContext& he) : he(he){};

  virtual ~AbstractCiphertext(){};

  AbstractCiphertext& operator=(const AbstractCiphertext& src) = delete;

  std::shared_ptr<AbstractCiphertext> clone() const
  {
    return std::static_pointer_cast<AbstractCiphertext>(doClone());
  }

  inline HeContext& getContext() { return he; }

  inline const HeContext& getContext() const { return he; }

  virtual std::streamoff save(std::ostream& stream) const = 0;

  virtual std::streamoff load(std::istream& stream) = 0;

  virtual void add(const AbstractCiphertext& other) = 0;

  virtual void addRaw(const AbstractCiphertext& other) = 0;

  virtual void sub(const AbstractCiphertext& other) = 0;

  virtual void subRaw(const AbstractCiphertext& other) = 0;

  virtual void multiply(const AbstractCiphertext& other) = 0;

  virtual void multiplyRaw(const AbstractCiphertext& other) = 0;

  virtual void addPlain(const AbstractPlaintext& plain) = 0;

  virtual void addPlainRaw(const AbstractPlaintext& plain) = 0;

  virtual void subPlain(const AbstractPlaintext& plain) = 0;

  virtual void subPlainRaw(const AbstractPlaintext& plain) = 0;

  virtual void multiplyPlain(const AbstractPlaintext& plain) = 0;

  virtual void multiplyPlainRaw(const AbstractPlaintext& plain) = 0;

  virtual void square() = 0;

  virtual void squareRaw() = 0;

  virtual void addScalar(int scalar);

  virtual void addScalar(double scalar);

  virtual void multiplyScalar(int scalar);

  virtual void multiplyScalar(double scalar);

  virtual void relinearize() = 0;

  virtual void rescale() = 0;

  virtual void rescaleRaw() = 0;

  virtual void setScale(double scale) = 0;

  virtual double getScale() const = 0;

  virtual void conjugate() = 0;

  virtual void conjugateRaw() = 0;

  virtual void negate() = 0;

  // rotate left
  virtual void rotate(int n) = 0;

  // inner sum of 0..n-1, cyclic
  // todo: rename sum
  virtual void innerSum(int n);

  // performs additions with copies of this vector
  // rotated by: rot1, rot1*2, rot1*4, ... up to and excluding rot2
  // if reverse is true, it rotates right instead of left
  virtual void innerSum(int rot1, int rot2, bool reverse = false);

  virtual void reduceChainIndex() = 0;

  virtual void setChainIndex(const AbstractCiphertext& other) = 0;

  virtual void setChainIndex(int chainIndex) = 0;

  virtual int getChainIndex() const = 0;

  virtual int slotCount() const = 0;

  virtual void debugPrint(const std::string& title = "",
                          int maxElements = -1,
                          int verbose = 0,
                          std::ostream& out = std::cout) const;

  // inner sum of 0..n-1, cyclic.
  // Follows total sums algorithm from "Algorithms in HElib" paper, section 4.1.
  virtual void sumExpBySquaringLeftToRight(int n);

  // inner sum of 0..n-1, cyclic
  virtual void sumExpBySquaringRightToLeft(int n);

  virtual void multiplyByChangingScale(double factor);

  virtual bool isEmpty() const = 0;
};
}

#endif /* SRC_HELAYERS_ABSTRACTCIPHER_H_ */
