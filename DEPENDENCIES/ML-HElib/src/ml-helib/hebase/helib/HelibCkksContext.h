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

#ifndef SRC_HEDGE_HELIBCKKSCONTEXT_H_
#define SRC_HEDGE_HELIBCKKSCONTEXT_H_

#include "HelibContext.h"

/** A wrapper class over some of HElib's main classes.
 *
 * It can be either initialized via an HelibConfig, or loaded from a file.
 *
 * All other classes (e.g., CTile or CipherMatrix) hold a reference to it,
 * for access to the keys and other services.
 *
 * It may or may not hold the secret key.
 * If initialized via config, it will contain both secret and public key.
 * Using file I/O it can be specified whether to include the secret key or not.
 * Server side contexts are not supposed to have the secret key.
 */

namespace hedge {

class HelibCkksContext : public HelibContext
{

  const helib::EncryptedArrayCx* ea = NULL;

public:
  HelibCkksContext();
  virtual ~HelibCkksContext();

  void init(unsigned long m, unsigned long r, unsigned long L);
  void init(const HelibConfig& conf);

  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher();

  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain();

  std::shared_ptr<AbstractEncoder> getEncoder() override;

  inline const helib::EncryptedArrayCx& getEncryptedArray() { return *ea; }

  void load(std::istream& out) override;

  std::string getSchemeName() const override { return "CKKS"; }
};
}

#endif /* SRC_HEDGE_HELIBCKKSCONTEXT_H_ */
