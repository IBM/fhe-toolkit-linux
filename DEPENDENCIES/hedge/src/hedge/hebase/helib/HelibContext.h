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

#ifndef SRC_HEDGE_HELIBCONTEXT_H_
#define SRC_HEDGE_HELIBCONTEXT_H_

#include "hedge/hebase/HeContext.h"
#include "helib/helib.h"
#include "HelibConfig.h"

namespace hedge {

class HelibContext : public HeContext
{
protected:
  HelibConfig config;

  helib::Context* context = NULL;
  helib::SecKey* secretKey = NULL;
  helib::PubKey* publicKey = NULL;

  long nslots = 0;

  bool mirrored = false;

public:
  HelibContext();
  virtual ~HelibContext();
  int getTopChainIndex() const override;

  inline int slotCount() const override { return nslots; }

  inline int getSecurityLevel() const override
  {
    return (int)(context->securityLevel());
  }

  inline bool hasSecretKey() const override { return secretKey != NULL; }

  inline const helib::PubKey& getPublicKey() const { return *publicKey; }
  inline const helib::SecKey& getSecretKey() const
  {
    if (!hasSecretKey())
      throw std::runtime_error("This context does not have a secret key");
    return *secretKey;
  }
  inline const helib::Context& getContext() const { return *context; }

  inline bool getMirrored() const { return mirrored; }
  inline void setMirrored(bool v) { mirrored = v; }

  void printSignature(std::ostream& out = std::cout) const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  void save(std::ostream& out, bool withSecretKey) override;

  void load(std::istream& in) override;

  void saveSecretKey(std::ostream& out) override;

  void loadSecretKey(std::istream& in) override;

  std::string getLibraryName() const override { return "HELIB"; }
};
}

#endif /* SRC_HEDGE_HELIBCONTEXT_H_ */
