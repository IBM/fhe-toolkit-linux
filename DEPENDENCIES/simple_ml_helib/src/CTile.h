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

#ifndef SRC_GENERIC_CTILE_H
#define SRC_GENERIC_CTILE_H

#include "HeContext.h"
#include "AbstractCiphertext.h"
#include "PTile.h"

/** A class representing a ciphertext.
 * Usually users of this library will work with higher-level classes
 * such as CipherMatrix, which internally holds multpile CTile-s.
 */
class CTile {

  	std::shared_ptr<AbstractCiphertext> impl;

	friend class AbstractEncoder;

public:

  CTile(HeContext& he);

  CTile(const CTile& src);

  ~CTile();

  CTile& operator=(const CTile& src);

  std::streamoff save(std::ostream& stream) const;

  std::streamoff load(std::istream& stream);

  void square();

  // Square in-place without relin/rescale
  void squareRaw();

  void conjugate();

  // rotate right
  void rotate(int n);

  void innerSum(int rot1, int rot2, bool reverse = false);

  // inner sum of 0..n-1, cyclic.
  // Follows total sums algorithm from "Algorithms in HElib" paper, section 4.1.
  void sumExpBySquaringLeftToRight(int n);

  // inner sum of 0..n-1, cyclic
  void sumExpBySquaringRightToLeft(int n);

  void add(const CTile& other);

  void sub(const CTile& other);

  // element-wise
  void multiply(const CTile& other);

  void multiplyPlain(const PTile& plain);

  void addAndAdjustChainIndex(const CTile& other);

  void subAndAdjustChainIndex(const CTile& other);

  void multiplyAndAdjustChainIndex(const CTile& other);

  void multiplyPlainAndAdjustChainIndex(const PTile& plain);

  //multiply without relin/rescale
  void multiplyRaw(const CTile& other);

  void relinearize();

  void rescale();

  void reduceChainIndex();

  void setChainIndex(const CTile& other);

  void setChainIndex(int chainIndex);

  int getChainIndex() const;

  int slotCount() const;

  void debugPrint(const std::string& title = "", int maxElements = 4, std::ostream& out = std::cout) const;

  double assertEquals(const std::string& title, const std::vector<double>& expectedVals, double eps = 1e-6, bool percent=false) const;

  std::shared_ptr<AbstractCiphertext> getImpl() { return impl; }; // decprecated

};

#endif /* SRC_GENERIC_CTILE_H */
