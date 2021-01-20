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

#include "HelibBgvEncoder.h"
#include "HelibBgvCiphertext.h"
#include "HelibBgvPlaintext.h"

using namespace std;
using namespace helib;

namespace helayers {

HelibBgvEncoder::HelibBgvEncoder(HelibBgvContext& he) : he(he) {}

HelibBgvEncoder::~HelibBgvEncoder() {}

void HelibBgvEncoder::encode(AbstractPlaintext& res,
                             const std::vector<int>& vals,
                             int chainIndex) const
{
  HelibBgvPlaintext& p = dynamic_cast<HelibBgvPlaintext&>(res);
  for (int i_vals = 0; i_vals < he.slotCount(); ++i_vals) {
    int i_p = he.getMirrored() ? he.slotCount() - i_vals - 1 : i_vals;
    p.pt[i_p] = (i_vals < vals.size() ? (vals[i_vals]) : 0);
  }
}
void HelibBgvEncoder::encode(AbstractPlaintext& res,
                             const std::vector<long>& vals,
                             int chainIndex) const
{
  HelibBgvPlaintext& p = dynamic_cast<HelibBgvPlaintext&>(res);
  for (int i_vals = 0; i_vals < he.slotCount(); ++i_vals) {
    int i_p = he.getMirrored() ? he.slotCount() - i_vals - 1 : i_vals;
    p.pt[i_p] = (i_vals < vals.size() ? (vals[i_vals]) : 0);
  }
}

void HelibBgvEncoder::encode(AbstractPlaintext& res,
                             const vector<double>& vals,
                             int chainIndex) const
{
  HelibBgvPlaintext& p = dynamic_cast<HelibBgvPlaintext&>(res);
  for (int i_vals = 0; i_vals < he.slotCount(); ++i_vals) {
    int i_p = he.getMirrored() ? he.slotCount() - i_vals - 1 : i_vals;
    p.pt[i_p] = (i_vals < vals.size() ? (vals[i_vals]) : 0);
  }
}

void HelibBgvEncoder::encode(AbstractPlaintext& res,
                             const vector<complex<double>>& vals,
                             int chainIndex) const
{
  throw runtime_error("BGV scheme does not support encoding of complex values");
}

vector<long> HelibBgvEncoder::decodeLong(const AbstractPlaintext& src) const
{
  const HelibBgvPlaintext& p = dynamic_cast<const HelibBgvPlaintext&>(src);
  vector<long> res(he.slotCount());
  for (int i_slot = 0; i_slot < he.slotCount(); ++i_slot) {
    int i_vec = (he.getMirrored()) ? he.slotCount() - i_slot - 1 : i_slot;
    res[i_vec] = (long)p.pt[i_slot];
  }
  return res;
}

vector<int> HelibBgvEncoder::decodeInt(const AbstractPlaintext& src) const
{
  const HelibBgvPlaintext& p = dynamic_cast<const HelibBgvPlaintext&>(src);
  vector<int> res(he.slotCount());
  for (int i_slot = 0; i_slot < he.slotCount(); ++i_slot) {
    int i_vec = (he.getMirrored()) ? he.slotCount() - i_slot - 1 : i_slot;
    res[i_vec] = (long)p.pt[i_slot];
  }
  return res;
}

vector<double> HelibBgvEncoder::decodeDouble(const AbstractPlaintext& src) const
{
  const HelibBgvPlaintext& p = dynamic_cast<const HelibBgvPlaintext&>(src);
  vector<double> res(he.slotCount());
  for (int i_slot = 0; i_slot < he.slotCount(); ++i_slot) {
    int i_vec = (he.getMirrored()) ? he.slotCount() - i_slot - 1 : i_slot;
    res[i_vec] = (long)p.pt[i_slot];
  }
  return res;
}

vector<complex<double>> HelibBgvEncoder::decodeComplex(
    const AbstractPlaintext& src) const
{
  std::vector<double> vals = decodeDouble(src);
  std::vector<complex<double>> convertedVals;
  for (const auto& val : vals)
    convertedVals.push_back(val);
  return convertedVals;
}

void HelibBgvEncoder::encrypt(AbstractCiphertext& res,
                              const AbstractPlaintext& src) const
{
  HelibBgvCiphertext& c = dynamic_cast<HelibBgvCiphertext&>(res);
  const HelibBgvPlaintext& p = dynamic_cast<const HelibBgvPlaintext&>(src);
  he.getPublicKey().Encrypt(c.ctxt, p.pt);
}

void HelibBgvEncoder::decrypt(AbstractPlaintext& res,
                              const AbstractCiphertext& src) const
{
  const HelibBgvCiphertext& c = dynamic_cast<const HelibBgvCiphertext&>(src);
  HelibBgvPlaintext& p = dynamic_cast<HelibBgvPlaintext&>(res);
  he.getSecretKey().Decrypt(p.pt, c.ctxt);
}

void HelibBgvEncoder::encodeEncrypt(AbstractCiphertext& res,
                                    const vector<int>& vals,
                                    int chainIndex) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  encode(*p, vals, chainIndex);
  encrypt(res, *p);
}
void HelibBgvEncoder::encodeEncrypt(AbstractCiphertext& res,
                                    const vector<long>& vals,
                                    int chainIndex) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  encode(*p, vals, chainIndex);
  encrypt(res, *p);
}

void HelibBgvEncoder::encodeEncrypt(AbstractCiphertext& res,
                                    const vector<double>& vals,
                                    int chainIndex) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  encode(*p, vals, chainIndex);
  encrypt(res, *p);
}

void HelibBgvEncoder::encodeEncrypt(AbstractCiphertext& res,
                                    const vector<complex<double>>& vals,
                                    int chainIndex) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  encode(*p, vals, chainIndex);
  encrypt(res, *p);
}

vector<int> HelibBgvEncoder::decryptDecodeInt(
    const AbstractCiphertext& src) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  decrypt(*p, src);
  return decodeInt(*p);
}

vector<long> HelibBgvEncoder::decryptDecodeLong(
    const AbstractCiphertext& src) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  decrypt(*p, src);
  return decodeLong(*p);
}

vector<double> HelibBgvEncoder::decryptDecodeDouble(
    const AbstractCiphertext& src) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  decrypt(*p, src);
  return decodeDouble(*p);
}

vector<complex<double>> HelibBgvEncoder::decryptDecodeComplex(
    const AbstractCiphertext& src) const
{
  shared_ptr<AbstractPlaintext> p = he.createAbstractPlain();
  decrypt(*p, src);
  return decodeComplex(*p);
}
}
