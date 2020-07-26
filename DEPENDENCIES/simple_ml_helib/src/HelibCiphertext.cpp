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

#include "HelibCiphertext.h"

using namespace std;

void HelibCiphertext::encrypt(const AbstractPlaintext& p) {
  throw runtime_error("not implemented");
}


streamoff HelibCiphertext::save(ostream& stream) const {
  streampos streamStartPos = stream.tellp();

  ctxt.write(stream);

  streampos streamEndPos = stream.tellp();

  return streamEndPos - streamStartPos;
}

streamoff HelibCiphertext::load(istream& stream)  {
  streampos streamStartPos = stream.tellg();

  ctxt.read(stream);

  streampos streamEndPos = stream.tellg();

  return streamEndPos - streamStartPos;
}

void HelibCiphertext::square() {
  multiply(*this);
}


void HelibCiphertext::squareRaw() {
  square();
}


void HelibCiphertext::add(const AbstractCiphertext& other) {
  const HelibCiphertext& castedOther = dynamic_cast<const HelibCiphertext&>(other);
  ctxt+=castedOther.ctxt;
}

void HelibCiphertext::sub(const AbstractCiphertext& other) {
  const HelibCiphertext& castedOther = dynamic_cast<const HelibCiphertext&>(other);
  ctxt-=castedOther.ctxt;
}

void HelibCiphertext::multiply(const AbstractCiphertext& other) {
  const HelibCiphertext& castedOther = dynamic_cast<const HelibCiphertext&>(other);
  ctxt.multiplyBy(castedOther.ctxt);
}

void HelibCiphertext::multiplyRaw(const AbstractCiphertext& other) {
  const HelibCiphertext& castedOther = dynamic_cast<const HelibCiphertext&>(other);
  ctxt*=castedOther.ctxt;
}

void HelibCiphertext::relinearize() {
  ctxt.reLinearize();
}


void HelibCiphertext::reduceChainIndex() {

}

void HelibCiphertext::setChainIndex(const AbstractCiphertext& other) {

}

void HelibCiphertext::setChainIndex(int chainIndex) {

}

void HelibCiphertext::rescale() {
}

int HelibCiphertext::getChainIndex() const {
  return 0;
}



