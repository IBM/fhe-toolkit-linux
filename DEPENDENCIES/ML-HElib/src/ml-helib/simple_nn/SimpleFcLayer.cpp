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

#include "SimpleFcLayer.h"
#include "CipherMatrixEncoder.h"

using namespace std;
using namespace boost::numeric::ublas;

namespace hedge {

SimpleFcLayer::SimpleFcLayer(HeContext& he) : he(he), weights(he), bias(he) {}

SimpleFcLayer::~SimpleFcLayer() {}

streamoff SimpleFcLayer::save(ostream& stream) const
{
  HEDGE_TIMER_PUSH("SimpleFcLayer_" + getName());
  HEDGE_TIMER_PUSH("SimpleFcLayer::save");

  streampos streamStartPos = stream.tellp();

  weights.save(stream);
  bias.save(stream);

  streampos streamEndPos = stream.tellp();

  HEDGE_TIMER_POP();
  HEDGE_TIMER_POP();
  return streamEndPos - streamStartPos;
}

streamoff SimpleFcLayer::load(istream& stream)
{
  HEDGE_TIMER_PUSH("SimpleFcLayer_" + getName());
  HEDGE_TIMER_PUSH("SimpleFcLayer::load");

  streampos streamStartPos = stream.tellg();

  weights.load(stream);
  bias.load(stream);

  streampos streamEndPos = stream.tellg();

  HEDGE_TIMER_POP();
  HEDGE_TIMER_POP();
  return streamEndPos - streamStartPos;
}

void SimpleFcLayer::initFromLayer(const SimpleFcPlainLayer& fpl,
                                  int baseChainIndex)
{
  HEDGE_TIMER_PUSH("SimpleFcLayer_" + getName());
  HEDGE_TIMER_PUSH("SimpleFcLayer::initFromLayer");

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    if (baseChainIndex < -1 || baseChainIndex > he.getTopChainIndex())
      throw invalid_argument("Illegal chain index value");
    if (baseChainIndex == -1)
      baseChainIndex = he.getTopChainIndex();
  }

  tensor<double> weightsVals = fpl.getWeights().getTensor();
  tensor<double> biasVals = fpl.getBias().getTensor();

  const CipherMatrixEncoder encoder(he);
  encoder.encodeEncrypt(weights, weightsVals, baseChainIndex);
  encoder.encodeEncrypt(bias, biasVals, baseChainIndex - 1);

  HEDGE_TIMER_POP();
  HEDGE_TIMER_POP();
}

CipherMatrix SimpleFcLayer::forward(const CipherMatrix& inVec) const
{
  HEDGE_TIMER_PUSH("SimpleFcLayer_" + getName());
  HEDGE_TIMER_PUSH("SimpleFcLayer::forward");

  CipherMatrix res = weights.getMatrixMultiply(inVec);
  res.add(bias);

  HEDGE_TIMER_POP();
  HEDGE_TIMER_POP();
  return res;
}
}
