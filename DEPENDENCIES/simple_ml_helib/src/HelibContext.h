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

#ifndef SRC_ML_HE_EXAMPLES_HELIBCONTEXT_H_
#define SRC_ML_HE_EXAMPLES_HELIBCONTEXT_H_

#include "HeContext.h"
#include "helib/helib.h"
#include "HelibConfig.h"

class HelibContext : public HeContext {
protected:
  HelibConfig config;

  helib::Context *context=NULL;
  helib::SecKey* secretKey=NULL;
  helib::PubKey* publicKey=NULL;

  long nslots=0;

  bool mirrored=false;
public:
  HelibContext();
  virtual ~HelibContext();
  int getTopChainIndex() const override;

  inline int slotCount() const override {
    return nslots;
  }

  inline int getSecurityLevel() const override {
      return (int)(context->securityLevel());
  }

  bool hasSecretKey() const override;

  inline const helib::PubKey& getPublicKey() { return *publicKey;}
  inline const helib::SecKey& getSecretKey() { return *secretKey;}
  inline const helib::Context& getContext() { return *context; }

  inline bool getMirrored() { return mirrored; }
  inline void setMirrored(bool v) { mirrored=v; }

  bool automaticallyManagesChainIndices()  const override { return true; }

  void save(std::ostream&out,bool withSecretKey) override;

  void load(std::istream&in) override;

};

#endif /* SRC_ML_HE_EXAMPLES_HELIBCONTEXT_H_ */
