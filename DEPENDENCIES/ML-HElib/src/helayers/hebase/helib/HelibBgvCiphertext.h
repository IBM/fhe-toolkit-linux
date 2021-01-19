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

#ifndef SRC_HELAYERS_HELIBBGVCIPHER_H_
#define SRC_HELAYERS_HELIBBGVCIPHER_H_

#include "HelibBgvContext.h"
#include "HelibCiphertext.h"

namespace helayers {

class HelibBgvCiphertext : public HelibCiphertext
{
  HelibBgvContext& he;

  HelibBgvCiphertext(const HelibBgvCiphertext& src) = default;

  std::shared_ptr<AbstractCiphertext> doClone() const override;

  friend class HelibBgvEncoder;
  friend class HelibBgvNativeFunctionEvaluator;

public:
  HelibBgvCiphertext(HelibBgvContext& h) : HelibCiphertext(h), he(h) {}

  virtual ~HelibBgvCiphertext();

  std::shared_ptr<HelibBgvCiphertext> clone() const
  {
    return std::static_pointer_cast<HelibBgvCiphertext>(doClone());
  }

  void addPlainRaw(const AbstractPlaintext& plain) override;

  void subPlainRaw(const AbstractPlaintext& plain) override;

  void multiplyPlainRaw(const AbstractPlaintext& plain) override;

  void conjugate() override;

  void conjugateRaw() override;

  // rotate right
  void rotate(int n) override;

  void negate() override;

  int slotCount() const override;
};
}

#endif /* SRC_HELAYERS_HELIBBGVCIPHER_H_ */
