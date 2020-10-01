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

#include "TrainingSetPlain.h"
#include "h5Parser.h"
#include <cassert>
#include <iostream>

using namespace std;

namespace hedge {

void TrainingSetPlain::loadSamples(const std::string& sampleFile,
                                   const std::string& sampleWeights)
{
  H5Parser h5(sampleFile);

  vector<double> raw;
  vector<int> dims;
  h5.readData(sampleWeights, raw, dims);

  while (dims.size() < 4)
    dims.push_back(1);

  assert(dims.size() == 4);
  numSamples = dims[0];
  int inputRows = dims[1];
  int inputCols = dims[2];
  assert(dims[3] == 1);

  batches.resize(ceil((double)numSamples / batchSize));

  int currentBatch = -1;
  int pos = 0;
  for (int i = 0; i < numSamples; ++i) {
    if ((i % batchSize) == 0) {
      currentBatch++;
      batches[currentBatch].samples.init(inputRows, inputCols, batchSize);
    }
    for (int x = 0; x < inputRows; ++x) {
      for (int y = 0; y < inputCols; ++y) {
        batches[currentBatch]
            .samples.getMat(i % batchSize)
            .set(x, y, raw[pos++]);
      }
    }
  }
}

void TrainingSetPlain::loadLabels(const std::string& labelFile,
                                  const std::string& labelWeights)
{
  vector<double> raw;
  vector<int> dims;
  H5Parser h5b(labelFile);
  h5b.readData(labelWeights, raw, dims);
  assert(dims.size() == 2);
  assert(dims[0] == numSamples);
  numLabels = dims[1];
  batches.resize(ceil((double)numSamples / batchSize));
  int pos = 0;
  int currentBatch = -1;
  for (int i = 0; i < numSamples; ++i) {
    if ((i % batchSize) == 0) {
      currentBatch++;
      batches[currentBatch].labels.init(numLabels, 1, batchSize);
    }
    for (int y = 0; y < numLabels; ++y) {
      batches[currentBatch].labels.getMat(i % batchSize).set(y, 0, raw[pos++]);
    }
  }
}

void TrainingSetPlain::loadFromH5(const std::string& sampleFile,
                                  const std::string& sampleWeights,
                                  const std::string& labelFile,
                                  const std::string& labelWeights)
{
  loadSamples(sampleFile, sampleWeights);
  loadLabels(labelFile, labelWeights);
}

void TrainingSetPlain::loadFromH5(const std::string& sampleFile,
                                  const std::string& sampleWeights)
{
  loadSamples(sampleFile, sampleWeights);
}

DoubleMatrixArray TrainingSetPlain::getAllSamples() const
{
  DoubleMatrixArray res;
  for (auto const& batch : batches) {
    for (size_t i = 0; i < batch.samples.size(); ++i)
      res.pushBackMatrix(batch.samples.getMat(i));
  }
  return res;
}

DoubleMatrixArray TrainingSetPlain::getAllLabels() const
{
  DoubleMatrixArray res;
  for (auto const& batch : batches) {
    for (size_t i = 0; i < batch.labels.size(); ++i)
      res.pushBackMatrix(batch.labels.getMat(i));
  }
  return res;
}
}
