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

#include "HelibBgvContext.h"
#include "HelibBgvCiphertext.h"
#include "HelibBgvPlaintext.h"
#include "HelibBgvEncoder.h"
#include "HelibBgvNativeFunctionEvaluator.h"

using namespace std;
using namespace helib;

namespace helayers {

HelibBgvContext::HelibBgvContext()
{
  traits.setAutomaticallyManagesChainIndices(true);
  traits.setAutomaticallyManagesRescale(true);
  traits.setIsModularArithmetic(true);
}

HelibBgvContext::~HelibBgvContext() {}

void HelibBgvContext::init(unsigned long _p,
                           unsigned long _m,
                           unsigned long _r,
                           unsigned long _L)
{
  HelibConfig conf;
  conf.p = _p;
  conf.m = _m;
  conf.r = _r;
  conf.L = _L;
  init(conf);
}

void HelibBgvContext::init(const HelibConfig& conf)
{
  if (context != NULL)
    throw runtime_error("This context is already initialized");

  config = conf;
  traits.setArithmeticModulus(config.p);
  context = new Context(config.m, config.p, config.r);
  buildModChain(*context, config.L, config.c);
  secretKey = new helib::SecKey(*context);
  secretKey->GenSecKey();
  addSome1DMatrices(*secretKey);
  publicKey = secretKey;
  ea = context->ea;
  nslots = ea->size();
}

void HelibBgvContext::init(const HelibConfig& conf,
                           helib::Context* userContext,
                           helib::SecKey* userSecretKey,
                           helib::PubKey* userPublicKey)
{
  config = conf;
  context = userContext;
  secretKey = userSecretKey;
  publicKey = userPublicKey;
  ea = context->ea;
  nslots = ea->size();
}

shared_ptr<AbstractCiphertext> HelibBgvContext::createAbstractCipher()
{
  return make_shared<HelibBgvCiphertext>(*this);
}

shared_ptr<AbstractPlaintext> HelibBgvContext::createAbstractPlain()
{
  return make_shared<HelibBgvPlaintext>(*this);
}

shared_ptr<AbstractEncoder> HelibBgvContext::getEncoder()
{
  return make_shared<HelibBgvEncoder>(*this);
}

shared_ptr<AbstractFunctionEvaluator> HelibBgvContext::getFunctionEvaluator()
{
  return make_shared<HelibBgvNativeFunctionEvaluator>(*this);
}

void HelibBgvContext::printSignature(std::ostream& out) const
{
  out << "HElib BGV" << endl;
  out << config << endl;
  out << "SecurityLevel=" << getSecurityLevel() << endl;
  out << "Slots=" << slotCount() << endl;
}

void HelibBgvContext::load(std::istream& in)
{
  HelibContext::load(in);
  ea = context->ea;
  nslots = ea->size();
}
}
