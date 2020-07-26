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

#include "HelibCkksPlaintext.h"

using namespace std;

HelibCkksPlaintext::HelibCkksPlaintext(const HelibCkksPlaintext& src) :
    heContext(src.heContext), pt(src.pt) {
}

shared_ptr<AbstractPlaintext> HelibCkksPlaintext::doClone() const {
    return make_shared<HelibCkksPlaintext>(*this);
}

void HelibCkksPlaintext::encode(const vector<double>& vals, int givenChainIndex) {
  for (int i=0;i<slotCount();++i)
    pt[i]=(i<vals.size())?vals[i]:0;
}

void HelibCkksPlaintext::encode(const vector<complex<double>>& vals, int givenChainIndex) {
   for (int i=0;i<slotCount();++i)
     pt[i]=(i<vals.size())?vals[i]:0;
}

vector<double> HelibCkksPlaintext::decode() const {
  vector<double> res;
  for (int i=0;i<slotCount();++i)
    res.push_back(pt[i].real());
  return res;
}

vector<complex<double>> HelibCkksPlaintext::decodeComplex() const {
	vector<complex<double>> res;
  for (int i=0;i<slotCount();++i)
    res.push_back(pt[i]);
  return res;
}

void HelibCkksPlaintext::reduceChainIndex() {

}

void HelibCkksPlaintext::setChainIndex(const AbstractPlaintext& other) {

}

void HelibCkksPlaintext::setChainIndex(int givenChainIndex) {

}

int HelibCkksPlaintext::getChainIndex() const {
    return 0;
}

int HelibCkksPlaintext::slotCount() const {
  return heContext.slotCount();
}

void HelibCkksPlaintext::debugPrint(const string& title, int maxVals, ostream& out) const {
  vector<double> res = decode();
  out << title << ": " << " slots=" << res.size() << endl;
  if (maxVals < 0)
    maxVals = res.size();
  for(int i=0; i<min(maxVals, (int) res.size()); ++i)
    out << " " << res[i];
  out << endl;
}

const helib::Ptxt<helib::CKKS>& HelibCkksPlaintext::getPlaintext() const {

  return pt;
}

