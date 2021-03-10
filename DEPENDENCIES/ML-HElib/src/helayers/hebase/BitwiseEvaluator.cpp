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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "BitwiseEvaluator.h"

using namespace std;

namespace helayers {

BitwiseEvaluator::BitwiseEvaluator(HeContext& h)
    : h(h), impl(h.getBitwiseEvaluator())
{
  if (!h.getTraits().getSupportsBitwiseOperations()) {
    throw runtime_error("Can't create BitwiseEvaluator with an HeContext that"
                        " does not support bitwise operations. Such a context "
                        "included in full helayers version.");
  }
}

CTile BitwiseEvaluator::getMSB(const CTile& c) const
{
  CTile res(h);
  res.impl = impl->getMSB(*c.impl);
  return res;
}

CTile BitwiseEvaluator::getFlippedMSB(const CTile& c) const
{
  CTile res(h);
  res.impl = impl->getFlippedMSB(*c.impl);
  return res;
}

void BitwiseEvaluator::setIsSigned(CTile& c, bool val) const
{
  impl->setIsSigned(*c.impl, val);
}

bool BitwiseEvaluator::getIsSigned(const CTile& c) const
{
  return impl->getIsSigned(*c.impl);
}

CTile BitwiseEvaluator::hamming(const CTile& c, int from, int to) const
{
  CTile res(h);
  res.impl = impl->hamming(*c.impl);
  return res;
}

vector<CTile> BitwiseEvaluator::split(const CTile& c) const
{
  vector<shared_ptr<AbstractCiphertext>> res = impl->split(*c.impl);

  vector<CTile> resCTileVec(res.size(), CTile(h));
  for (size_t i = 0; i < res.size(); i++) {
    resCTileVec[i].impl = res[i];
  }

  return resCTileVec;
}

CTile BitwiseEvaluator::combine(const vector<CTile> cs,
                                int from,
                                int to,
                                int bitsPerElement) const
{
  vector<shared_ptr<AbstractCiphertext>> csCasted;
  csCasted.reserve(cs.size());

  for (const auto& c : cs) {
    csCasted.push_back(c.impl);
  }

  CTile res(h);
  res.impl = impl->combine(csCasted, from, to, bitsPerElement);
  return res;
}

CTile BitwiseEvaluator::isEqual(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->isEqual(*c1.impl, *c2.impl);
  return res;
}

CTile BitwiseEvaluator::multiply(const CTile& c1,
                                 const CTile& c2,
                                 int targetBits) const
{
  CTile res(h);
  res.impl = impl->multiply(*c1.impl, *c2.impl, targetBits);
  return res;
}

CTile BitwiseEvaluator::add(const CTile& c1,
                            const CTile& c2,
                            int targetBits) const
{
  CTile res(h);
  res.impl = impl->add(*c1.impl, *c2.impl, targetBits);
  return res;
}

CTile BitwiseEvaluator::sub(const CTile& c1,
                            const CTile& c2,
                            int targetBits) const
{
  CTile res(h);
  res.impl = impl->sub(*c1.impl, *c2.impl, targetBits);
  return res;
}

CTile BitwiseEvaluator::multiplyBit(const CTile& c, const CTile& bit) const
{
  CTile res(h);
  res.impl = impl->multiplyBit(*c.impl, *(bit.impl));
  return res;
}

CTile BitwiseEvaluator::bitwiseXor(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->bitwiseXor(*c1.impl, *c2.impl);
  return res;
}

int BitwiseEvaluator::getNumBits(const CTile& c) const
{
  return impl->getNumBits(*c.impl);
}

void BitwiseEvaluator::setNumBits(CTile& c, int bits) const
{
  impl->setNumBits(*c.impl, bits);
}

int BitwiseEvaluator::getDefaultNumBits() const
{
  return impl->getDefaultNumBits();
}

void BitwiseEvaluator::debugPrintWithBinary(const CTile& c,
                                            const string& title,
                                            int maxElements,
                                            int verbose,
                                            ostream& out) const
{
  impl->debugPrintWithBinary(*c.impl, title, maxElements, verbose, out);
}

double BitwiseEvaluator::getScale(const CTile& c) const
{
  return impl->getScale(*c.impl);
}

CTile BitwiseEvaluator::max(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->max(*c1.impl, *c2.impl);
  return res;
}
CTile BitwiseEvaluator::min(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->min(*c1.impl, *c2.impl);
  return res;
}

CTile BitwiseEvaluator::isGreater(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->isGreater(*c1.impl, *c2.impl);
  return res;
}
CTile BitwiseEvaluator::isLess(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->isLess(*c1.impl, *c2.impl);
  return res;
}
CTile BitwiseEvaluator::isGreaterEqual(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->isGreaterEqual(*c1.impl, *c2.impl);
  return res;
}
CTile BitwiseEvaluator::isLessEqual(const CTile& c1, const CTile& c2) const
{
  CTile res(h);
  res.impl = impl->isLessEqual(*c1.impl, *c2.impl);
  return res;
}
} // namespace helayers
