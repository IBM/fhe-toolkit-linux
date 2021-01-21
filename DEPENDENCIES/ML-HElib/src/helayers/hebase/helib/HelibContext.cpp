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

#include "HelibContext.h"
#include "HelibCkksContext.h"
#include "HelibBgvContext.h"

using namespace std;
using namespace helib;

namespace helayers {

HelibContext::HelibContext() : HeContext()
{
  // TODO Auto-generated constructor stub
}

HelibContext::~HelibContext()
{
  // TODO Auto-generated destructor stub
}

shared_ptr<HelibContext> HelibContext::create(HelibPreset preset)
{
  HelibConfig conf;
  conf.initPreset(preset);
  shared_ptr<HelibContext> he;
  if (conf.p == -1)
    he = make_shared<HelibCkksContext>();
  else
    he = make_shared<HelibBgvContext>();

  he->init(conf);
  return he;
}

void HelibContext::init(const HelibConfig& conf)
{
  throw runtime_error("not implemented");
}

void HelibContext::initPreset(HelibPreset preset)
{
  HelibConfig conf;
  conf.initPreset(preset);
  init(conf);
}

int HelibContext::getTopChainIndex() const
{
  // cout << "WARNING: getTopChainIndex() is unsupported. returned arbitrary
  // value" << endl;
  return -1;
}

void HelibContext::printSignature(std::ostream& out) const
{
  out << "HElib " << getSchemeName() << " "
      << (hasSecretKey() ? "(WITH SECRET KEY)" : "(NO SECRET KEY)") << endl;
  out << config << endl;
  out << "SecurityLevel=" << getSecurityLevel() << endl;
}

void HelibContext::debugPrint(const std::string& title,
                              int verbose,
                              std::ostream& out) const
{
  printSignature(out);
}

void HelibContext::save(std::ostream& out, bool withSecretKey)
{
  HeContext::save(out, withSecretKey);

  config.save(out);
  out.write((char*)&withSecretKey, sizeof(withSecretKey));
  out.write((char*)&mirrored, sizeof(mirrored));
  context->writeTo(out);
  // writeContextBinary(out, *context);
  if (withSecretKey)
    secretKey->writeTo(out);
  else
    publicKey->writeTo(out);
}

void HelibContext::saveSecretKey(std::ostream& out)
{
  if (!hasSecretKey()) {
    throw runtime_error("this HeContext does not have a secret key");
  }
  secretKey->writeTo(out);
}

void HelibContext::loadSecretKey(std::istream& in)
{
  if (hasSecretKey()) {
    throw runtime_error("this HeContext already has a secret key");
  }
  secretKey = new SecKey(SecKey::readFrom(in, *context));
}

void HelibContext::load(std::istream& in)
{
  HeContext::load(in);

  if (context != NULL)
    throw runtime_error("This context is already initialized");
  config.load(in);
  bool withSecretKey;
  in.read((char*)&withSecretKey, sizeof(withSecretKey));
  in.read((char*)&mirrored, sizeof(mirrored));
  context = Context::readPtrFrom(in);
  // readContextBinary(in, *context);

  if (withSecretKey) {
    secretKey = new SecKey(SecKey::readFrom(in, *context));
    publicKey = (helib::PubKey*)secretKey;
  } else {

    publicKey = new PubKey(PubKey::readFrom(in, *context));
    // readPubKeyBinary(in, *publicKey);
    secretKey = NULL;
  }
}

void HelibContext::init(const HeConfigRequirement& req)
{
  throw runtime_error("not implemented");
}

bool HelibContext::isConfigRequirementFeasible(
    const HeConfigRequirement& req) const
{
  throw runtime_error("not implemented");
}
}
