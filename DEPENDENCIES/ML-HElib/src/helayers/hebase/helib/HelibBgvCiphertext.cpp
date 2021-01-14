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

#include "HelibBgvCiphertext.h"
#include "HelibBgvPlaintext.h"
#include "helayers/hebase/HelayersTimer.h"

using namespace std;

namespace helayers {

HelibBgvCiphertext::~HelibBgvCiphertext()
{
  // TODO Auto-generated destructor stub
}

shared_ptr<AbstractCiphertext> HelibBgvCiphertext::doClone() const
{
  HELAYERS_TIMER("HelibBgvCiphertext::doClone");
  return shared_ptr<HelibBgvCiphertext>(new HelibBgvCiphertext(*this));
}

void HelibBgvCiphertext::addPlainRaw(const AbstractPlaintext& p)
{
  HELAYERS_TIMER("HelibBgvCiphertext::addPlainRaw");
  const HelibBgvPlaintext& castedOther =
      dynamic_cast<const HelibBgvPlaintext&>(p);
  ctxt.addConstant(castedOther.getPlaintext());
}

void HelibBgvCiphertext::subPlainRaw(const AbstractPlaintext& p)
{
  HELAYERS_TIMER("HelibBgvCiphertext::subPlainRaw");
  const HelibBgvPlaintext& castedOther =
      dynamic_cast<const HelibBgvPlaintext&>(p);
  helib::Ptxt<helib::BGV> ptxtCopy(castedOther.getPlaintext());
  ctxt.addConstant(ptxtCopy.negate());
}

void HelibBgvCiphertext::multiplyPlainRaw(const AbstractPlaintext& p)
{
  HELAYERS_TIMER("HelibBgvCiphertext::multiplyPlainRaw");
  const HelibBgvPlaintext& castedOther =
      dynamic_cast<const HelibBgvPlaintext&>(p);
  ctxt.multByConstant(castedOther.getPlaintext());
}

void HelibBgvCiphertext::negate()
{
  HELAYERS_TIMER("HelibBgvCiphertext::negate");
  ctxt.negate();
}

void HelibBgvCiphertext::conjugate()
{
  // does nothing
}

void HelibBgvCiphertext::conjugateRaw()
{
  // does nothing
}

void HelibBgvCiphertext::rotate(int n)
{
  HELAYERS_TIMER("HelibBgvCiphertext::rotate");
  if (he.getMirrored())
    he.getEncryptedArray().rotate(ctxt, n);
  else
    he.getEncryptedArray().rotate(ctxt, -n);
}

int HelibBgvCiphertext::slotCount() const { return he.slotCount(); }
}
