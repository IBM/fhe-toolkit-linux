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

#include "boost/filesystem.hpp"
#include "Layer.h"
#include "SimpleTimer.h"
#include "AlwaysAssert.h"
#include <sys/stat.h>
#include "h5Parser.h"

using namespace std;

const std::string Layer::FLOW_TITLE="FLOW: ";

ostream& operator<<(ostream&out, const Layer&l) {
  l.debugPrint(out);
  return out;
}

PlainLayer::~PlainLayer() {
}

void PlainLayer::debugPrint(ostream& out, int verboseLevel) const {
  out << "Layer " << getName() << endl;
  vector<DoubleMatrixArray> weights = getAllWeightsPlain();
  for (int i = 0; i < (int) weights.size(); ++i) {
    out << "Weights_" << i << endl;
    out << weights[i];
  }
}


void PlainLayer::clear() {
  for (DoubleMatrixArray* plainWeightsMat : getAllWeightsRefsPlain()) {
    plainWeightsMat->clear();
  }
}

DoubleMatrixArray SquareActivationPlainLayer::forward(
    const DoubleMatrixArray& inVec) {
  DoubleMatrixArray res(inVec);
  res.elementMultiply(res);
  return res;
}



