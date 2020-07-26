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

#include "FcLayer.h"
#include "SimpleTimer.h"
#include "AlwaysAssert.h"

using namespace std;

static DoubleMatrix loadFcMatrixFromH5File(const H5Parser& h5P,
    const string& path) {
  vector<vector<double> > FcMatrix = h5P.parseFC(path);

  DoubleMatrix res(FcMatrix);

  // Transpose "FcMatrix", after reading them from the h5 file provided by Keras.
  // Note that when Fc layers matrices are used in multiplications in Keras, the weights
  // matrix is in the right side of the multiplication. While in multiplications executed by
  // nn_train, the weights matrix is in the left side.
  // Therefore, we transpose the weights matrices provided by Keras in order to ensure the results
  // of all multiplications will be the same (up to transposition).
  res.transpose();

  return res;
}


FcPlainLayer::FcPlainLayer() {
}

vector<DoubleMatrixArray> FcPlainLayer::getAllWeightsPlain() const {
  vector<DoubleMatrixArray> res { weights, bias };
  return res;
}

vector<DoubleMatrixArray*> FcPlainLayer::getAllWeightsRefsPlain() {
  vector<DoubleMatrixArray*> res { &weights, &bias };
  return res;
}

void FcPlainLayer::initSize(int rows, int cols, int filledSlots) {
  weights.init(rows, cols, filledSlots);
  bias.init(rows, 1, filledSlots);
}

void FcPlainLayer::initWeightsRandom() {
  weights.initRandomSame();
  bias.initRandomSame();
}

void FcPlainLayer::initWeightsConst(double c) {
  weights.initConst(c / weights.cols());
}

DoubleMatrixArray FcPlainLayer::forward(const DoubleMatrixArray& inVec) {
  SimpleTimer st("FcPlainLayer::forward");

  DoubleMatrixArray lastInput = inVec;
  if (lastInput.rows()==1 && lastInput.cols()>1)
    lastInput.transpose();
  DoubleMatrixArray res = weights.getMatrixMultiply(lastInput);
  res.add(bias);
  return res;
}


void FcPlainLayer::loadh5(const H5Parser& h5P) {
  string path=getName()+"/"+getName();
  for(int i=0; i<weights.size(); ++i){
    weights.getMat(i).copy(loadFcMatrixFromH5File(h5P, path));
    bias.getMat(i).unFlatten(h5P.parseBias(path));
  }
}

void FcPlainLayer::saveh5Fc(H5Dumper& h5D, const DoubleMatrix&weightsMat,
    const DoubleMatrix&biasMat, const string&layerName) {
  // Get bias as a two dimensional vector of doubles.
  // Note that the returned two dimensional vector is expected to contain only one row.
  vector<vector<double> > biasVec2D = biasMat.getTranspose().getVals();

  if (biasVec2D.size() != 1)
    throw runtime_error(
        "Expecting bias to have one row, instead found "
        + to_string(biasVec2D.size()));
  vector<double> biasVec = biasVec2D[0];

  string biasName = layerName + "/bias:0";
  string denseName = layerName + "/kernel:0";
  vector<string> weightNames = {biasName, denseName};

  h5D.setLayerWeightNames(layerName, weightNames);
  h5D.dumpBias(biasVec, layerName);
  h5D.dumpFC(weightsMat.getTranspose().getVals(), layerName);
}

void FcPlainLayer::saveh5(int slot, H5Dumper& h5D) const {
  saveh5Fc(h5D, weights.getMat(slot), bias.getMat(slot), getName());
}


