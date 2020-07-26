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

#ifndef SRC_ML_HE_EXAMPLES_ABSTRACTPLAIN_H_
#define SRC_ML_HE_EXAMPLES_ABSTRACTPLAIN_H_

#include <vector>
#include <memory>
#include <iostream>
#include <complex>

class AbstractPlaintext {

	virtual std::shared_ptr<AbstractPlaintext> doClone() const = 0;

public:

  AbstractPlaintext() {};

  virtual ~AbstractPlaintext() {};

  std::shared_ptr<AbstractPlaintext> clone() const {
  	return std::static_pointer_cast<AbstractPlaintext>(doClone());
  }

  virtual void encode(const std::vector<double>& vals, int chainIndex = -1) = 0;

  virtual void encode(const std::vector<std::complex<double>>& vals, int chainIndex = -1) = 0;

  // in case of complex values, returns real parts only
  virtual std::vector<double> decode() const = 0;

  virtual std::vector<std::complex<double>> decodeComplex() const = 0;

  virtual void reduceChainIndex() = 0;

  virtual void setChainIndex(const AbstractPlaintext& other) = 0;

  virtual void setChainIndex(int chainIndex) = 0;

  virtual int getChainIndex() const = 0;

  virtual int slotCount() const = 0;

  virtual void debugPrint(const std::string& title = "", int maxElements = -1, std::ostream& out = std::cout) const = 0;

};

#endif /* SRC_ML_HE_EXAMPLES_ABSTRACTPLAIN_H_ */
