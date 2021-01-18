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
#include "helayers/hebase/HelayersTimer.h"
#include "helayers/hebase/AlwaysAssert.h"

using namespace helib;
using namespace std;

namespace helayers {

REGISTER_HE_CONTEXT(HelibCkksContext);

HelibCkksContext::HelibCkksContext()
{
  traits.setAutomaticallyManagesChainIndices(true);
  traits.setAutomaticallyManagesRescale(true);
  traits.setSupportsComplexNumbers(true);
  traits.setSupportsDecryptAddedNoise(true);
}

HelibCkksContext::~HelibCkksContext()
{
  // TODO Auto-generated destructor stub
}

std::shared_ptr<HeContext> HelibCkksContext::clone() const
{
  return make_shared<HelibCkksContext>();
}

void HelibCkksContext::init(unsigned long m,
                            unsigned long r,
                            unsigned long L,
                            unsigned long c,
                            bool enableConjugate)
{
  if (context != NULL)
    throw runtime_error("This context is already initialized");

  HelibConfig hc;
  hc.m = m;
  hc.L = L;
  hc.r = r;
  hc.c = c;
  hc.enableConjugate = enableConjugate;
  init(hc);
}

void HelibCkksContext::init(const HelibConfig& conf)
{
  HELAYERS_TIMER_SECTION("context-init");
  if (context != NULL)
    throw runtime_error("This context is already initialized");
  always_assert(conf.p == -1, "p must be set to -1 in CKKS");

  config = conf;
  unsigned long p = -1;

  context = new Context(config.m, p, config.r);
  buildModChain(*context, config.L, config.c);
  secretKey = new helib::SecKey(*context);
  secretKey->GenSecKey();
  addSome1DMatrices(*secretKey);
  if (conf.enableConjugate) {
    addFrbMatrices(*secretKey);
  }
  publicKey = secretKey;

  initCommon(context);
}

void HelibCkksContext::init(const HelibConfig& conf,
                            helib::Context* userContext,
                            helib::SecKey* userSecretKey,
                            helib::PubKey* userPublicKey)
{
  if (context != NULL)
    throw runtime_error("This context is already initialized");
  always_assert(conf.p == -1, "p must be set to -1 in CKKS");

  context = userContext;
  secretKey = userSecretKey;
  publicKey = userPublicKey;
  config = conf;
  initCommon(context);
}

void HelibCkksContext::initCommon(helib::Context* context)
{
  ea = &context->ea->getCx();
  nslots = ea->size();
}

shared_ptr<AbstractCiphertext> HelibCkksContext::createAbstractCipher()
{
  return make_shared<HelibCkksCiphertext>(*this);
}

shared_ptr<AbstractPlaintext> HelibCkksContext::createAbstractPlain()
{
  return make_shared<HelibCkksPlaintext>(*this);
}

shared_ptr<AbstractEncoder> HelibCkksContext::getEncoder()
{
  return make_shared<HelibCkksEncoder>(*this);
}

void HelibCkksContext::load(std::istream& in)
{
  HelibContext::load(in);
  ea = &context->ea->getCx();
  nslots = ea->size();
}
}
