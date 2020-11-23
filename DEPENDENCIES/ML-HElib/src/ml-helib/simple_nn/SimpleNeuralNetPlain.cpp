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

#include "SimpleNeuralNetPlain.h"

using namespace std;

namespace hedge {

SimpleNeuralNetPlain::SimpleNeuralNetPlain() {}

SimpleNeuralNetPlain::~SimpleNeuralNetPlain() {}

void SimpleNeuralNetPlain::loadh5(const H5Parser& h5p,
                                  std::vector<string> names,
                                  std::vector<int> dims,
                                  int numFilledSlots)
{
  if (names.size() != 3)
    throw invalid_argument("3 names for 3 FC layers are required");
  if (dims.size() != 4)
    throw invalid_argument("4 dimensions (a,b,c,d) for 3 FC layers (of "
                           "dimensions a*b, b*c, c*d) are required");

  fpl1.setName(names.at(0));
  fpl1.initSize(dims.at(1), dims.at(0), numFilledSlots);
  fpl1.loadh5(h5p);
  fpl2.setName(names.at(1));
  fpl2.initSize(dims.at(2), dims.at(1), numFilledSlots);
  fpl2.loadh5(h5p);
  fpl3.setName(names.at(2));
  fpl3.initSize(dims.at(3), dims.at(2), numFilledSlots);
  fpl3.loadh5(h5p);
}

void SimpleNeuralNetPlain::predict(const DoubleMatrixArray& input,
                                   DoubleMatrixArray& output) const
{
  output = sapl.forward(fpl3.forward(
      sapl.forward(fpl2.forward(sapl.forward(fpl1.forward(input))))));
}
}
