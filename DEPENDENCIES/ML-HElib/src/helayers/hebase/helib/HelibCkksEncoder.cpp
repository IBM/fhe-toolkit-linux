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

namespace helayers {

HelibCkksEncoder::HelibCkksEncoder(HelibCkksContext& he) : he(he) {}

HelibCkksEncoder::~HelibCkksEncoder() {}

void HelibCkksEncoder::setDecryptAddedNoiseEnabled(bool val)
{
  decryptAddedNoiseEnabled = val;
}

bool HelibCkksEncoder::getDecryptAddedNoiseEnabled() const
{
  return decryptAddedNoiseEnabled;
}

void HelibCkksEncoder::setDecryptAddedNoisePrecision(int val)
{
  decryptAddedNoisePrecision = val;
}

int HelibCkksEncoder::getDecryptAddedNoisePrecision() const
{
  return decryptAddedNoisePrecision;
}

void HelibCkksEncoder::encode(AbstractPlaintext& res,
                              const vector<double>& vals,
                              int chainIndex) const
{
  HelibCkksPlaintext& p = dynamic_cast<HelibCkksPlaintext&>(res);
  for (int i_val = 0; i_val < he.slotCount(); ++i_val) {
    int i_slot = (he.getMirrored()) ? he.slotCount() - i_val - 1 : i_val;
    p.pt[i_slot] = (i_val < vals.size() ? vals[i_val] : 0);
  }
}

void HelibCkksEncoder::encode(AbstractPlaintext& res,
                              const vector<complex<double>>& vals,
                              int chainIndex) const
{
  HelibCkksPlaintext& p = dynamic_cast<HelibCkksPlaintext&>(res);
  for (int i_val = 0; i_val < he.slotCount(); ++i_val) {
    int i_slot = (he.getMirrored()) ? he.slotCount() - i_val - 1 : i_val;
    p.pt[i_slot] = (i_val < vals.size() ? vals[i_val] : 0);
  }
}

vector<double> HelibCkksEncoder::decodeDouble(
    const AbstractPlaintext& src) const
{
  const HelibCkksPlaintext& p = dynamic_cast<const HelibCkksPlaintext&>(src);
  vector<double> res(he.slotCount());
  for (int i = 0; i < he.slotCount(); ++i)
    if (he.getMirrored())
      res[he.slotCount() - i - 1] = p.pt[i].real();
    else
      res[i] = p.pt[i].real();
  return res;
}

vector<complex<double>> HelibCkksEncoder::decodeComplex(
    const AbstractPlaintext& src) const
{
  const HelibCkksPlaintext& p = dynamic_cast<const HelibCkksPlaintext&>(src);
  vector<complex<double>> res(he.slotCount());
  for (int i = 0; i < he.slotCount(); ++i)
    if (he.getMirrored())
      res[he.slotCount() - i - 1] = p.pt[i];
    else
      res[i] = p.pt[i];
  return res;
}

void HelibCkksEncoder::encrypt(AbstractCiphertext& res,
                               const AbstractPlaintext& src) const
{
  HelibCkksCiphertext& c = dynamic_cast<HelibCkksCiphertext&>(res);
  const HelibCkksPlaintext& p = dynamic_cast<const HelibCkksPlaintext&>(src);
  he.getEncryptedArray().encrypt(c.ctxt, he.getPublicKey(), p.pt);
}

void HelibCkksEncoder::decrypt(AbstractPlaintext& res,
                               const AbstractCiphertext& src) const
{
  const HelibCkksCiphertext& c = dynamic_cast<const HelibCkksCiphertext&>(src);
  HelibCkksPlaintext& p = dynamic_cast<HelibCkksPlaintext&>(res);

  if (!decryptAddedNoiseEnabled) {
    vector<complex<double>> res(he.slotCount());
    he.getEncryptedArray().rawDecrypt(c.ctxt, he.getSecretKey(), res);
    for (int i = 0; i < he.slotCount(); ++i)
      p.pt[i] = res[i];
  } else if (decryptAddedNoisePrecision > 0)
    he.getEncryptedArray().decrypt(
        c.ctxt, he.getSecretKey(), p.pt, decryptAddedNoisePrecision);
  else
    he.getEncryptedArray().decrypt(c.ctxt, he.getSecretKey(), p.pt);
}

void HelibCkksEncoder::encodeEncrypt(AbstractCiphertext& res,
                                     const vector<double>& vals,
                                     int chainIndex) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  encode(*p, vals, chainIndex);
  encrypt(res, *p);
}

void HelibCkksEncoder::encodeEncrypt(AbstractCiphertext& res,
                                     const vector<complex<double>>& vals,
                                     int chainIndex) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  encode(*p, vals, chainIndex);
  encrypt(res, *p);
}

vector<double> HelibCkksEncoder::decryptDecodeDouble(
    const AbstractCiphertext& src) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  decrypt(*p, src);
  return decodeDouble(*p);
}

vector<complex<double>> HelibCkksEncoder::decryptDecodeComplex(
    const AbstractCiphertext& src) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  decrypt(*p, src);
  return decodeComplex(*p);
}
}
