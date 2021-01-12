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

#ifndef SRC_HELAYERS_SIMPLE_LAYER_H
#define SRC_HELAYERS_SIMPLE_LAYER_H

#include "helayers/simple_nn/DoubleMatrixArray.h"
#include "helayers/simple_nn/h5Parser.h"

namespace helayers {

/// A class representing a single neural network layer
class SimpleLayer
{
  std::string name;
  int index;

public:
  virtual ~SimpleLayer() {}

  /// Sets this layer name.
  /// @param[in] n name
  inline void setName(const std::string& n) { name = n; }

  /// Returns layer name
  inline const std::string& getName() const { return name; }

  /// Sets this layer index (order in network).
  /// @param[in] i index
  inline void setIndex(int i) { index = i; }

  /// Returns this layer index (order in network).
  inline int getIndex() const { return index; }
};
}

#endif /* SRC_HELAYERS_SIMPLE_LAYER_H */
