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

#include "HelibCkksContext.h"

#include "HelibCkksCiphertext.h"
#include "HelibCkksPlaintext.h"
#include "HelibCkksEncoder.h"

using namespace helib;
using namespace std;

HelibCkksContext::HelibCkksContext() { }
  
HelibCkksContext::~HelibCkksContext() { }
  
void HelibCkksContext::init(unsigned long m,unsigned long r,unsigned long L) {
  HelibConfig hc;
  hc.m=m;
  hc.L=L;
  hc.r=r;
  init(hc);
}

void HelibCkksContext::init(const HelibConfig& conf) {
  config=conf;
  unsigned long p = -1;


  context=new Context(config.m, p, config.r);
  buildModChain(*context, config.L, config.c);
  // Print the context
  //  context->zMStar.printout();
  //  std::cout << std::endl;

  secretKey=new helib::SecKey(*context);

  secretKey->GenSecKey();

  addSome1DMatrices(*secretKey);

  publicKey=secretKey;


  ea = &context->ea->getCx();


  nslots = ea->size();
  //  std::cout << "Number of slots: " << nslots << std::endl;
}


shared_ptr<AbstractCiphertext> HelibCkksContext::createAbstractCipher() {
  return make_shared<HelibCkksCiphertext>(*this);
}

shared_ptr<AbstractPlaintext> HelibCkksContext::createAbstractPlain() {
  return make_shared<HelibCkksPlaintext>(*this);
}

shared_ptr<AbstractEncoder> HelibCkksContext::getEncoder() {
  return make_shared<HelibCkksEncoder>(*this);
}

void HelibCkksContext::printSignature(std::ostream&out) const {
  out << "HElib CKKS "  
    << (hasSecretKey()?"(WITH SECRET KEY)":"(NO SECRET KEY)") 
    << endl;
  out << config << endl;
  out << "SecurityLevel=" << getSecurityLevel() << endl;
}

void HelibCkksContext::load(std::istream&in) {
  HelibContext::load(in);
  ea = &context->ea->getCx();
  nslots = ea->size();
}
