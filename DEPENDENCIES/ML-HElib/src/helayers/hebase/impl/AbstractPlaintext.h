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

#ifndef SRC_HELAYERS_ABSTRACTPLAIN_H_
#define SRC_HELAYERS_ABSTRACTPLAIN_H_

#include <vector>
#include <memory>
#include <iostream>
#include <complex>

#include "helayers/hebase/HeContext.h"

namespace helayers {

///@brief Internal abstract implementation of a plaintext, do not use directly.
/// Used as parent class for concrete classes implementing plaintexts in
/// various schemes.
/// Do not use directly, use a PTile class instead.
class AbstractPlaintext
{

  HeContext& he;

  virtual std::shared_ptr<AbstractPlaintext> doClone() const = 0;

protected:
  AbstractPlaintext(const AbstractPlaintext& src) = default;

public:
  AbstractPlaintext(HeContext& he) : he(he){};

  virtual ~AbstractPlaintext(){};

  AbstractPlaintext& operator=(const AbstractPlaintext& src) = delete;

  std::shared_ptr<AbstractPlaintext> clone() const
  {
    return std::static_pointer_cast<AbstractPlaintext>(doClone());
  }

  inline HeContext& getContext() { return he; }

  virtual std::streamoff save(std::ostream& stream) const = 0;

  virtual std::streamoff load(std::istream& stream) = 0;

  inline const HeContext& getContext() const { return he; }

  virtual void reduceChainIndex() = 0;

  virtual void setChainIndex(const AbstractPlaintext& other) = 0;

  virtual void setChainIndex(int chainIndex) = 0;

  virtual int getChainIndex() const = 0;

  virtual int slotCount() const = 0;

  virtual void debugPrint(const std::string& title = "",
                          int maxElements = -1,
                          int verbose = 0,
                          std::ostream& out = std::cout) const;
};
}

#endif /* SRC_HELAYERS_ABSTRACTPLAIN_H_ */
