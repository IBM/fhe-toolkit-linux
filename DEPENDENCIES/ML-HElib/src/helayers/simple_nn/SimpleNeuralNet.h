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

#ifndef SRC_HELAYERS_SIMPNEURALNET_H
#define SRC_HELAYERS_SIMPNEURALNET_H

#include "SimpleFcLayer.h"
#include "SimpleSquareActivationLayer.h"
#include "CipherMatrix.h"
#include "SimpleNeuralNetPlain.h"
#include "helayers/hebase/utils/Saveable.h"

namespace helayers {

/** A simple neural network with a fixed architecture:
 * 3 fully connected layers, each followed by a square activation layer.
 * It works on encrypted data: both inputs and weights,
 * stored as CipherMatrix.
 *
 * The encrypted weights of this network are either loaded from file,
 * or encrypted from a SimpleNeuralNetPlain.
 */
class SimpleNeuralNet : public Saveable
{

  HeContext& he;

  SimpleFcLayer fcl1;
  SimpleFcLayer fcl2;
  SimpleFcLayer fcl3;
  SimpleSquareActivationLayer sal;

public:
  /// Construct a network.
  /// @param[in] he the underlying context.
  SimpleNeuralNet(HeContext& he);

  ~SimpleNeuralNet();

  /// Save network to binary stream.
  /// @param[in] stream output stream to write to
  std::streamoff save(std::ostream& stream) const override;

  /// Load network from binary stream.
  /// @param[in] stream output stream to read from
  std::streamoff load(std::istream& stream) override;

  /// Init network from a plain network.
  /// @param[in] net network to init from.
  /// @param[in] baseChainIndex first chain index to use (where applicable).
  void initFromNet(const SimpleNeuralNetPlain& net, int baseChainIndex = -1);

  /// Run prediction.
  /// @param[in] input input data
  /// @param[out] output output prediction
  void predict(const CipherMatrix& input, CipherMatrix& output) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_SIMPNEURALNET_H */
