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

#include "SimpleNeuralNet.h"

using namespace std;

SimpleNeuralNet::SimpleNeuralNet(HeContext& he) :
		he(he), fcl1(he), fcl2(he), fcl3(he), sal() {
}

SimpleNeuralNet::~SimpleNeuralNet(){
}

streamoff SimpleNeuralNet::save(ostream& stream) const {
  streampos streamStartPos = stream.tellp();

  fcl1.save(stream);
  fcl2.save(stream);
  fcl3.save(stream);

  streampos streamEndPos = stream.tellp();

  return streamEndPos - streamStartPos;
}

streamoff SimpleNeuralNet::load(istream& stream){
  streampos streamStartPos = stream.tellg();

  fcl1.load(stream);
  fcl2.load(stream);
  fcl3.load(stream);

  streampos streamEndPos = stream.tellg();

  return streamEndPos - streamStartPos;
}

void SimpleNeuralNet::initFromNet(const SimpleNeuralNetPlain& net, int baseChainIndex){
	if(!he.automaticallyManagesChainIndices() && baseChainIndex == -1)
		baseChainIndex = he.getTopChainIndex();

	fcl1.initFromLayer(net.fpl1, baseChainIndex);
	fcl2.initFromLayer(net.fpl2, baseChainIndex - 2);
	fcl3.initFromLayer(net.fpl3, baseChainIndex - 4);
}

void SimpleNeuralNet::predict(const CipherMatrix& input, CipherMatrix& output) const {
	output = sal.forward(fcl3.forward(sal.forward(fcl2.forward(sal.forward(fcl1.forward(input))))));
}
