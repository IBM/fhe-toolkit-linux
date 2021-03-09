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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRC_HELAYERS_SIMPLE_FCPLAINLAYER_H
#define SRC_HELAYERS_SIMPLE_FCPLAINLAYER_H

#include "DoubleMatrixArray.h"
#include "h5Parser.h"
#include "SimpleLayer.h"

namespace helayers {

/// A class representing a non-encrypted fully connected layer
class SimpleFcPlainLayer : public SimpleLayer
{
  DoubleMatrixArray weights;
  DoubleMatrixArray bias;

public:
  SimpleFcPlainLayer();

  virtual ~SimpleFcPlainLayer() {}

  inline const DoubleMatrixArray& getWeights() const { return weights; }
  inline const DoubleMatrixArray& getBias() const { return bias; }

  /** Initializes with specified size, all zero weights */
  virtual void initSize(int rows, int cols, int filledSlots);

  virtual void initWeightsRandom();
  virtual void initWeightsConst(double c);

  // inVec should be a vertical vector of length 'cols'
  // return value is a vertical vector of length 'rows'
  virtual DoubleMatrixArray forward(const DoubleMatrixArray& inVec) const;

  virtual void loadh5(const H5Parser& h5File);
};
} // namespace helayers

#endif /* SRC_HELAYERS_SIMPLE_FCPLAINLAYER_H */
