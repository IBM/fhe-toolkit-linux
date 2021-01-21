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

#ifndef SRC_HELAYERS_HELIBBGVPLAIN_H_
#define SRC_HELAYERS_HELIBBGVPLAIN_H_

#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "HelibBgvContext.h"

namespace helayers {

class HelibBgvPlaintext : public AbstractPlaintext
{

  HelibBgvContext& heContext;

  helib::Ptxt<helib::BGV> pt;

  HelibBgvPlaintext(const HelibBgvPlaintext& src) = default;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class HelibBgvEncoder;

public:
  HelibBgvPlaintext(HelibBgvContext& h)
      : AbstractPlaintext(h), heContext(h), pt(h.getContext())
  {
  }

  ~HelibBgvPlaintext() override{};

  std::shared_ptr<HelibBgvPlaintext> clone() const
  {
    return std::static_pointer_cast<HelibBgvPlaintext>(doClone());
  }

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  void reduceChainIndex() override;

  void setChainIndex(const AbstractPlaintext& other) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  int slotCount() const override;

  const helib::Ptxt<helib::BGV>& getPlaintext() const;
};
}

#endif /* SRC_HELAYERS_HELIBBGVPLAIN_H_ */
