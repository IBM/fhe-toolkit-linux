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

#ifndef SRC_ML_HE_EXAMPLES_HELIBCIPHER_H_
#define SRC_ML_HE_EXAMPLES_HELIBCIPHER_H_

#include "AbstractCiphertext.h"
#include "HelibCkksContext.h"


class HelibCiphertext : public AbstractCiphertext {

protected:
  helib::Ctxt ctxt;
public:
  HelibCiphertext(HelibContext&h): ctxt(h.getPublicKey()) {}
  virtual ~HelibCiphertext() {}

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void encrypt(const AbstractPlaintext& p) override;

  void square() override;

  // Square in-place without relin/rescale
  void squareRaw() override;


  void add(const AbstractCiphertext& other) override;

  void sub(const AbstractCiphertext& other) override;

  // element-wise
  void multiply(const AbstractCiphertext& other) override;

  //multiply without relin/rescale
  void multiplyRaw(const AbstractCiphertext& other) override;

  void relinearize() override;

  void rescale() override;



  void reduceChainIndex() override;

  void setChainIndex(const AbstractCiphertext& other) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;



};

#endif /* SRC_ML_HE_EXAMPLES_HELIBCIPHER_H_ */
