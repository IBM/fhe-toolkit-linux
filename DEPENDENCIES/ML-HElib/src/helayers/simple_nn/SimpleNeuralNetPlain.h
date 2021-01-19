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

#ifndef SRC_HELAYERS_SIMPNEURALNETPLAIN_H
#define SRC_HELAYERS_SIMPNEURALNETPLAIN_H

#include "SimpleSquareActivationPlainLayer.h"
#include "SimpleFcPlainLayer.h"
#include "DoubleMatrixArray.h"
#include "h5Parser.h"

namespace helayers {

/** A simple neural network with a fixed architecture:
 * 3 fully connected layers, each followed by a square activation layer.
 * It works on plaintext data, stored as DoubleMatrixArray.
 */
class SimpleNeuralNetPlain
{
public:
  SimpleFcPlainLayer fpl1;
  SimpleFcPlainLayer fpl2;
  SimpleFcPlainLayer fpl3;
  SimpleSquareActivationPlainLayer sapl;

  friend class SimpleNeuralNet;

  /// Construct a network
  SimpleNeuralNetPlain();

  ~SimpleNeuralNetPlain();

  /// Loads weights.
  /// @param[in] h5p H5Parser used to read file
  /// @param[in] names names of layers
  /// @param[in] dims size of input vector for each layer, and size of final
  /// output
  ///@param[in] numFilledSlots batch size
  void loadh5(const H5Parser& h5p,
              std::vector<std::string> names,
              std::vector<int> dims,
              int numFilledSlots);

  /// Run prediction.
  /// @param[in] input input data
  /// @param[out] output output prediction
  void predict(const DoubleMatrixArray& input, DoubleMatrixArray& output) const;
};
}

#endif /* SRC_HELAYERS_SIMPNEURALNETPLAIN_H */
