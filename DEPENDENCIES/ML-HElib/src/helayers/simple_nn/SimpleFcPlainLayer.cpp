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

#include "SimpleFcPlainLayer.h"
#include "helayers/hebase/HelayersTimer.h"

using namespace std;

namespace helayers {

static DoubleMatrix loadFcMatrixFromH5File(const H5Parser& h5P,
                                           const string& path)
{
  vector<vector<double>> FcMatrix = h5P.parseFC(path);

  DoubleMatrix res(FcMatrix);

  // Transpose "FcMatrix", after reading them from the h5 file provided by
  // Keras.
  // Note that when Fc layers matrices are used in multiplications in Keras, the
  // weights
  // matrix is in the right side of the multiplication. While in multiplications
  // executed by
  // nn_train, the weights matrix is in the left side.
  // Therefore, we transpose the weights matrices provided by Keras in order to
  // ensure the results
  // of all multiplications will be the same (up to transposition).
  res.transpose();

  return res;
}

SimpleFcPlainLayer::SimpleFcPlainLayer() {}

void SimpleFcPlainLayer::initSize(int rows, int cols, int filledSlots)
{
  weights.init(rows, cols, filledSlots);
  bias.init(rows, 1, filledSlots);
}

void SimpleFcPlainLayer::initWeightsRandom()
{
  weights.initRandomSame();
  bias.initRandomSame();
}

void SimpleFcPlainLayer::initWeightsConst(double c)
{
  weights.initConst(c / weights.cols());
}

DoubleMatrixArray SimpleFcPlainLayer::forward(
    const DoubleMatrixArray& inVec) const
{
  HELAYERS_TIMER("SimpleFcPlainLayer::forward");

  DoubleMatrixArray lastInput = inVec;
  if (lastInput.rows() == 1 && lastInput.cols() > 1)
    lastInput.transpose();
  DoubleMatrixArray res = weights.getMatrixMultiply(lastInput);
  res.add(bias);
  return res;
}

void SimpleFcPlainLayer::loadh5(const H5Parser& h5P)
{
  string path = getName() + "/" + getName();
  for (size_t i = 0; i < weights.size(); ++i) {
    weights.getMat(i).copy(loadFcMatrixFromH5File(h5P, path));
    bias.getMat(i).unFlatten(h5P.parseBias(path));
  }
}
} // namespace helayers
