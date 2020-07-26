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

#ifndef STRAINER_FCLAYER_H_
#define STRAINER_FCLAYER_H_

#include "Layer.h"

class FcPlainLayer: public PlainLayer {
  DoubleMatrixArray weights;
  DoubleMatrixArray bias;
  
public:
  FcPlainLayer();

  virtual ~FcPlainLayer() {
  }

  inline const DoubleMatrixArray& getWeights() const {
    return weights;
  }
  inline const DoubleMatrixArray& getBias() const {
    return bias;
  }

  /** Initializes with specified size, all zero weights */
  virtual void initSize(int rows, int cols, int filledSlots);

  virtual void initWeightsRandom();
  virtual void initWeightsConst(double c);

  // inVec should be a vertical vector of length 'cols'
  // return value is a vertical vector of length 'rows'
  virtual DoubleMatrixArray forward(const DoubleMatrixArray& inVec);

  virtual std::vector<DoubleMatrixArray> getAllWeightsPlain() const;

  virtual void loadh5(const H5Parser& h5File);

  static void saveh5Fc(H5Dumper& h5File, const DoubleMatrix&weights,
      const DoubleMatrix&bias, const std::string&layerName);

  virtual void saveh5(int slot, H5Dumper& h5File) const;

protected:
  virtual std::vector<DoubleMatrixArray*> getAllWeightsRefsPlain();
};


#endif /* STRAINER_FCLAYER_H_ */
