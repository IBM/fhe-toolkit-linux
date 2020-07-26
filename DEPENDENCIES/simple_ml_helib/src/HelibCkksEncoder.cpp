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

#include "HelibCkksEncoder.h"
#include "HelibCkksCiphertext.h"
#include "HelibCkksPlaintext.h"

using namespace std;
using namespace helib;

HelibCkksEncoder::HelibCkksEncoder(HelibCkksContext& he) : he(he) {
}

HelibCkksEncoder::~HelibCkksEncoder(){
}

void HelibCkksEncoder::encode(PTile& res, const vector<double>& vals, int chainIndex) const {
	HelibCkksPlaintext& p = dynamic_cast<HelibCkksPlaintext&>(*getAbstractPlaintext(res));
  for(int i=0; i<he.slotCount(); ++i)
    p.pt[i] = (i < vals.size() ? vals[i] : 0);
}

void HelibCkksEncoder::encode(PTile& res, const vector<complex<double>>& vals, int chainIndex) const {
	HelibCkksPlaintext& p = dynamic_cast<HelibCkksPlaintext&>(*getAbstractPlaintext(res));
  for(int i=0; i<he.slotCount(); ++i)
    p.pt[i] = (i < vals.size() ? vals[i] : 0);
}

vector<double> HelibCkksEncoder::decodeDouble(const PTile& src) const {
	const HelibCkksPlaintext& p = dynamic_cast<const HelibCkksPlaintext&>(*getAbstractPlaintext(src));
	vector<double> res;
  for(int i=0; i<he.slotCount(); ++i)
    res.push_back(p.pt[i].real());
  return res;
}

vector<complex<double>> HelibCkksEncoder::decodeComplex(const PTile& src) const {
	const HelibCkksPlaintext& p = dynamic_cast<const HelibCkksPlaintext&>(*getAbstractPlaintext(src));
	vector<complex<double>> res;
  for(int i=0; i<he.slotCount(); ++i)
    res.push_back(p.pt[i]);
  return res;
}

void HelibCkksEncoder::encrypt(CTile& res, const PTile& src) const {
	HelibCkksCiphertext& c = dynamic_cast<HelibCkksCiphertext&>(*getAbstractCiphertext(res));
	const HelibCkksPlaintext& p = dynamic_cast<const HelibCkksPlaintext&>(*getAbstractPlaintext(src));
  he.getEncryptedArray().encrypt(c.ctxt, he.getPublicKey(), p.pt);
}

void HelibCkksEncoder::decrypt(PTile& res, const CTile& src) const {
	const HelibCkksCiphertext& c = dynamic_cast<const HelibCkksCiphertext&>(*getAbstractCiphertext(src));
	HelibCkksPlaintext& p = dynamic_cast<HelibCkksPlaintext&>(*getAbstractPlaintext(res));
  he.getEncryptedArray().decrypt(c.ctxt, he.getSecretKey(), p.pt);
}

void HelibCkksEncoder::encodeEncrypt(CTile& res, const vector<double>& vals, int chainIndex) const {
	PTile p(he);
	encode(p, vals, chainIndex);
	encrypt(res, p);
}

void HelibCkksEncoder::encodeEncrypt(CTile& res, const vector<complex<double>>& vals, int chainIndex) const {
	PTile p(he);
	encode(p, vals, chainIndex);
	encrypt(res, p);
}

vector<double> HelibCkksEncoder::decryptDecodeDouble(const CTile& src) const {
	PTile p(he);
	decrypt(p, src);
	return decodeDouble(p);
}

vector<complex<double>> HelibCkksEncoder::decryptDecodeComplex(const CTile& src) const {
	PTile p(he);
	decrypt(p, src);
	return decodeComplex(p);
}
