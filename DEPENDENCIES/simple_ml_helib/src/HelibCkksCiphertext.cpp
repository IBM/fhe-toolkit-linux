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

#include "HelibCkksCiphertext.h"

#include "HelibCkksPlaintext.h"

using namespace std;

HelibCkksCiphertext::~HelibCkksCiphertext()  {

}

HelibCkksCiphertext::HelibCkksCiphertext(const HelibCkksCiphertext& src) :
                HelibCiphertext(src),    he(src.he) {

}

shared_ptr<AbstractCiphertext> HelibCkksCiphertext::doClone() const {
  return make_shared<HelibCkksCiphertext>(*this);
}


void HelibCkksCiphertext::encrypt(const AbstractPlaintext& p) {
  const HelibCkksPlaintext& castedOther = dynamic_cast<const HelibCkksPlaintext&>(p);
  he.getEncryptedArray().encrypt(ctxt, he.getPublicKey(), castedOther.getRaw());
}

void HelibCkksCiphertext::encrypt(const vector<double>& vals, int chainIndex) {
  //  cout << "WARNING: ignoring chain index" << endl;
  he.getEncryptedArray().encrypt(ctxt, he.getPublicKey(), vals);
}

void HelibCkksCiphertext::encrypt(const vector<std::complex<double>>& vals, int chainIndex) {
  //  cout << "WARNING: ignoring chain index" << endl;
  he.getEncryptedArray().encrypt(ctxt, he.getPublicKey(), vals);
}

void HelibCkksCiphertext::multiplyPlain(const AbstractPlaintext& p) {
  const HelibCkksPlaintext& castedOther = dynamic_cast<const HelibCkksPlaintext&>(p);
  ctxt.multByConstantCKKS(castedOther.getPlaintext());
}


vector<double> HelibCkksCiphertext::decrypt() const {
  vector<double> res;
  he.getEncryptedArray().decrypt(ctxt, he.getSecretKey(), res);
  return res;
}

vector<complex<double>> HelibCkksCiphertext::decryptComplex() const {
	vector<complex<double>> res;
  he.getEncryptedArray().decrypt(ctxt, he.getSecretKey(), res);
  return res;
}

void HelibCkksCiphertext::conjugate() {
	ctxt.complexConj();
}

void HelibCkksCiphertext::rotate(int n) {
  he.getEncryptedArray().rotate(ctxt,-n);
}


int HelibCkksCiphertext::slotCount() const {
  return he.slotCount();
}


void HelibCkksCiphertext::debugPrint(const std::string& title, int maxElements, std::ostream& out) const {
  HelibCiphertext::debugPrint(title,maxElements,out);
//  ctxt.debugPrint(out);
}
