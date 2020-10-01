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

#ifndef SRC_HEDGE_SIMPNEURALNET_H
#define SRC_HEDGE_SIMPNEURALNET_H

#include "SimpleFcLayer.h"
#include "SimpleSquareActivationLayer.h"
#include "CipherMatrix.h"
#include "SimpleNeuralNetPlain.h"

namespace hedge {

/** A simple neural network with a fixed architecture:
 * 3 fully connected layers, each followed by a square activation layer.
 * It works on encrypted data: both inputs and weights,
 * stored as CipherMatrix.
 *
 * The encrypted weights of this network are either loaded from file,
 * or encrypted from a SimpleNeuralNetPlain.
 */
class SimpleNeuralNet
{

  HeContext& he;

  SimpleFcLayer fcl1;
  SimpleFcLayer fcl2;
  SimpleFcLayer fcl3;
  SimpleSquareActivationLayer sal;

public:
  SimpleNeuralNet(HeContext& he);

  ~SimpleNeuralNet();

  std::streamoff save(std::ostream& stream) const;

  std::streamoff load(std::istream& stream);

  void initFromNet(const SimpleNeuralNetPlain& net, int baseChainIndex = -1);

  void predict(const CipherMatrix& input, CipherMatrix& output) const;
};
}

#endif /* SRC_HEDGE_SIMPNEURALNET_H */
