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

#ifndef SRC_ML_HE_EXAMPLES_HELIBCKKSCIPHER_H_
#define SRC_ML_HE_EXAMPLES_HELIBCKKSCIPHER_H_


#include "HelibCiphertext.h"
#include "HelibCkksContext.h"

class HelibCkksCiphertext : public HelibCiphertext {
private:
  HelibCkksContext& he;

  std::shared_ptr<AbstractCiphertext> doClone() const override;

	friend class HelibCkksEncoder;

public:

  HelibCkksCiphertext(HelibCkksContext& h) :
    HelibCiphertext(h),he(h) {
      
  }

  HelibCkksCiphertext(const HelibCkksCiphertext& src);

  ~HelibCkksCiphertext() override;

  std::shared_ptr<HelibCkksCiphertext> clone() const {
  	return std::static_pointer_cast<HelibCkksCiphertext>(doClone());
  }

  void encrypt(const AbstractPlaintext& p) override;

  void encrypt(const std::vector<double>& vals, int chainIndex = -1) override;

  void encrypt(const std::vector<std::complex<double>>& vals, int chainIndex = -1) override;

  std::vector<double> decrypt() const override;

  std::vector<std::complex<double>> decryptComplex() const override;

  void multiplyPlain(const AbstractPlaintext& p) override;

  void conjugate() override;

  // rotate right
  void rotate(int n) override;

  int slotCount() const override;

  virtual void debugPrint(const std::string& title, int maxElements = 4, std::ostream& out = std::cout) const override;
};

#endif /* SRC_ML_HE_EXAMPLES_HELIBCKKSCIPHER_H_ */
