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

#include "CipherMatrixEncoder.h"
#include "SimpleTimer.h"

using namespace std;
using namespace boost::numeric::ublas;

CipherMatrixEncoder::CipherMatrixEncoder(HeContext& he) : he(he), enc(Encoder(he)) {
}

CipherMatrixEncoder::~CipherMatrixEncoder(){
}

void CipherMatrixEncoder::encodeEncrypt(CipherMatrix& res, const tensor<double>& vals,
		int chainIndex) const {
	SimpleTimer::Guard guard("CipherMatrixEncoder::encodeEncrypt");

	if(vals.order() != 3)
		throw invalid_argument("Input must be 3-dimensional tensor");
	if(vals.size(2) > res.prototype.slotCount())
		throw invalid_argument("Input has depth higher than the number of slots in CTile");

  int numRows = vals.size(0);
  int numCols = vals.size(1);
  int numFilledSlots = vals.size(2);

	basic_extents<size_t> extents(std::vector<size_t>{(long unsigned int)numRows,(long unsigned int)numCols});
	res.tiles = tensor<CTile>(extents, res.prototype);

  for(int i=0; i<numRows; i++){
    for(int j=0; j<numCols; j++){
    	std::vector<double> currentTileVals;
    	for(int k=0; k<res.prototype.slotCount(); k++)
    		currentTileVals.push_back(k < numFilledSlots ? vals.at(i,j,k) : 0);
    	enc.encodeEncrypt(res.tiles.at(i,j), currentTileVals, chainIndex);
    }
  }

  res.numFilledSlots = numFilledSlots;
}

void CipherMatrixEncoder::encodeEncrypt(CipherMatrix& res, const tensor<complex<double>>& vals,
		int chainIndex) const {
  throw runtime_error("not implemented");
}

tensor<double> CipherMatrixEncoder::decryptDecodeDouble(const CipherMatrix& src) const {
  SimpleTimer::Guard guard("CipherMatrixEncoder::decryptDecodeDouble");

  int numRows = src.tiles.size(0);
  int numCols = src.tiles.size(1);
  int numFilledSlots = src.numFilledSlots;

	tensor<double> res{(long unsigned int)numRows,(long unsigned int)numCols,(long unsigned int)numFilledSlots};

  for(int i=0; i<numRows; i++){
    for(int j=0; j<numCols; j++){
    	std::vector<double> currentTileVals = enc.decryptDecodeDouble(src.tiles.at(i,j));
    	for(int k=0; k<numFilledSlots; k++)
    		res.at(i,j,k) = currentTileVals.at(k);
    }
  }

  return res;
}

tensor<complex<double>> CipherMatrixEncoder::decryptDecodeComplex(const CipherMatrix& src) const {
  throw runtime_error("not implemented");
}
