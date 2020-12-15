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

#ifndef SRC_HELAYERS_HELIBCKKSCONTEXT_H_
#define SRC_HELAYERS_HELIBCKKSCONTEXT_H_

#include "HelibContext.h"

namespace helayers {

/** An implementation of HeContext for CKKS scheme in HElib.
 *
 * It can be either initialized via parameters, via an HelibConfig,
 * or loaded from a file.
 *
 * It is recommended not to use directly after initialization, but
 * use an HeContext reference instead.
 */
class HelibCkksContext : public HelibContext
{

  const helib::EncryptedArrayCx* ea = NULL;

public:
  /// Constructs an empty object.
  HelibCkksContext();

  virtual ~HelibCkksContext();

  /// Initializes context with given parameters
  void init(unsigned long m,
            unsigned long r,
            unsigned long L,
            unsigned long c = 2);

  /// Initialized context with given configuration
  void init(const HelibConfig& conf);

  /// Do not use. Should be made private.
  std::shared_ptr<AbstractCiphertext> createAbstractCipher() override;

  /// Do not use. Should be made private.
  std::shared_ptr<AbstractPlaintext> createAbstractPlain() override;

  /// Do not use. Should be made private.
  std::shared_ptr<AbstractEncoder> getEncoder() override;

  /// Returns encrypted array object
  inline const helib::EncryptedArrayCx& getEncryptedArray() { return *ea; }

  /// Loads from binary stream
  void load(std::istream& out) override;

  /// Returns scheme name
  std::string getSchemeName() const override { return "CKKS"; }
};
}

#endif /* SRC_HELAYERS_HELIBCKKSCONTEXT_H_ */
