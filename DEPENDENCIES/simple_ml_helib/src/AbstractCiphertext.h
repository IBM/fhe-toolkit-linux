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

#ifndef SRC_ML_HE_EXAMPLES_ABSTRACTCIPHER_H_
#define SRC_ML_HE_EXAMPLES_ABSTRACTCIPHER_H_

#include <vector>
#include <memory>
#include <iostream>
#include <complex>

#include "AbstractPlaintext.h"
#include "HeContext.h"

class AbstractCiphertext {

	virtual std::shared_ptr<AbstractCiphertext> doClone() const = 0;

public:

	AbstractCiphertext() {};

  virtual ~AbstractCiphertext() {};

  std::shared_ptr<AbstractCiphertext> clone() const {
  	return std::static_pointer_cast<AbstractCiphertext>(doClone());
  }

  virtual std::streamoff save(std::ostream& stream) const = 0;

  virtual std::streamoff load(std::istream& stream) = 0;

  virtual void encrypt(const std::vector<double>& vals, int chainIndex = -1) = 0;

  virtual void encrypt(const std::vector<std::complex<double>>& vals, int chainIndex = -1) = 0;

  virtual void encrypt(const AbstractPlaintext& p) = 0;

  virtual std::vector<double> decrypt() const = 0;

  virtual std::vector<std::complex<double>> decryptComplex() const = 0;

  virtual void add(const AbstractCiphertext& other) = 0;

  virtual void sub(const AbstractCiphertext& other) = 0;

  // element-wise
  virtual void multiply(const AbstractCiphertext& other) = 0;

  // multiply without relinearize and rescale
  virtual void multiplyRaw(const AbstractCiphertext& other) = 0;

  virtual void multiplyPlain(const AbstractPlaintext& plain) = 0;

  virtual void square() = 0;

  // square without relinearize and rescale
  virtual void squareRaw() = 0;

  virtual void relinearize() = 0;

  virtual void rescale() = 0;

  virtual void conjugate() = 0;

  // rotate left
  virtual void rotate(int n) = 0;

  // inner sum of 0..n-1, cyclic
  virtual void innerSum(int n);

  // performs additions with copies of this vector
  // rotated by: rot1, rot1*2, rot1*4, ... up to and excluding rot2
  // if reverse is true, it rotates right instead of left
  virtual void innerSum(int rot1, int rot2, bool reverse=false);

  virtual void reduceChainIndex() = 0;

  virtual void setChainIndex(const AbstractCiphertext& other) = 0;

  virtual void setChainIndex(int chainIndex) = 0;

  virtual int getChainIndex() const = 0;

  virtual int slotCount() const = 0;

  virtual void debugPrint(const std::string& title, int maxElements = 4, std::ostream& out = std::cout) const;

  // inner sum of 0..n-1, cyclic.
  // Follows total sums algorithm from "Algorithms in HElib" paper, section 4.1.
  virtual void sumExpBySquaringLeftToRight(int n);

  // inner sum of 0..n-1, cyclic
  virtual void sumExpBySquaringRightToLeft(int n);

  // tests that all elements within expectedVals are equal to the equivalent elements in this,
  // up to a given epsilon, and returns the max difference found
  virtual double assertEquals(const std::string& title, const std::vector<double>& expectedVals, double eps = 1e-6, bool percent=false) const;

  void multiplyPlainAndAdjustChainIndex(AbstractPlaintext& plain);

  void subAndAdjustChainIndex(AbstractCiphertext& other);

  void addAndAdjustChainIndex(AbstractCiphertext& other);

  void multiplyAndAdjustChainIndex(AbstractCiphertext& other);
};

#endif /* SRC_ML_HE_EXAMPLES_ABSTRACTCIPHER_H_ */
