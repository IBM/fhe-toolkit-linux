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

#ifndef SRC_ML_HE_EXAMPLES_HELIBCKKSPLAIN_H_
#define SRC_ML_HE_EXAMPLES_HELIBCKKSPLAIN_H_



#include "AbstractPlaintext.h"
#include "HelibCkksContext.h"


class HelibCkksPlaintext : public AbstractPlaintext {
  HelibCkksContext& heContext;


  helib::Ptxt<helib::CKKS> pt;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

	friend class HelibCkksEncoder;

public:

  HelibCkksPlaintext(HelibCkksContext& h) :
    heContext(h),pt(h.getContext()) {
  }

  HelibCkksPlaintext(const HelibCkksPlaintext& src);

  ~HelibCkksPlaintext() override {};

  std::shared_ptr<HelibCkksPlaintext> clone() const {
    return std::static_pointer_cast<HelibCkksPlaintext>(doClone());
  }

  void encode(const std::vector<double>& vals, int chainIndex = -1) override;

  void encode(const std::vector<std::complex<double>>& vals, int chainIndex = -1) override;

  std::vector<double> decode() const override;

  std::vector<std::complex<double>> decodeComplex() const override;

  void reduceChainIndex() override;

  void setChainIndex(const AbstractPlaintext& other) override;

  void setChainIndex(int chainIndex) override;

  int getChainIndex() const override;

  int slotCount() const override;

  void debugPrint(const std::string& title, int maxElements = 4, std::ostream& out = std::cout) const override;

  const helib::Ptxt<helib::CKKS>& getPlaintext() const;

  const helib::Ptxt<helib::CKKS>& getRaw() const { return pt;}
};

#endif /* SRC_ML_HE_EXAMPLES_HELIBCKKSPLAIN_H_ */
