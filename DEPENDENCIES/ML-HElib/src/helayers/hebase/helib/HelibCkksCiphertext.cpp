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

#include <NTL/ZZ.h>
#include "HelibCkksCiphertext.h"
#include "HelibCkksPlaintext.h"
#include "helayers/hebase/HelayersTimer.h"

using namespace std;

namespace helayers {

HelibCkksCiphertext::~HelibCkksCiphertext() {}

shared_ptr<AbstractCiphertext> HelibCkksCiphertext::doClone() const
{
  HELAYERS_TIMER("HelibCkksCiphertext::doClone");
  return shared_ptr<HelibCkksCiphertext>(new HelibCkksCiphertext(*this));
}

void HelibCkksCiphertext::addPlainRaw(const AbstractPlaintext& p)
{
  HELAYERS_TIMER("HelibCkksCiphertext::addPlainRaw");
  const HelibCkksPlaintext& castedOther =
      dynamic_cast<const HelibCkksPlaintext&>(p);
  ctxt += castedOther.getPlaintext();
}

void HelibCkksCiphertext::subPlainRaw(const AbstractPlaintext& p)
{
  HELAYERS_TIMER_SECTION("HelibCkksCiphertext::subPlainRaw");
  const HelibCkksPlaintext& castedOther =
      dynamic_cast<const HelibCkksPlaintext&>(p);
  ctxt -= castedOther.getPlaintext();
}

void HelibCkksCiphertext::multiplyPlainRaw(const AbstractPlaintext& p)
{
  HELAYERS_TIMER("HelibCkksCiphertext::multiplyPlainRaw");
  const HelibCkksPlaintext& castedOther =
      dynamic_cast<const HelibCkksPlaintext&>(p);
  ctxt *= castedOther.getPlaintext();
}

void HelibCkksCiphertext::addScalar(int scalar)
{
  HELAYERS_TIMER("HelibCkksCiphertext::addScalar(int)");
  ctxt += NTL::ZZ(scalar);
}

void HelibCkksCiphertext::addScalar(double scalar)
{
  HELAYERS_TIMER("HelibCkksCiphertext::addScalar(double)");
  ctxt += scalar;
}

void HelibCkksCiphertext::multiplyScalar(int scalar)
{
  HELAYERS_TIMER("HelibCkksCiphertext::multiplyScalar(int)");
  ctxt.multByConstant(NTL::ZZ(scalar));
}

void HelibCkksCiphertext::multiplyScalar(double scalar)
{
  HELAYERS_TIMER("HelibCkksCiphertext::multiplyScalar(double)");
  ctxt *= scalar;
}

void HelibCkksCiphertext::multiplyByChangingScale(double factor)
{
  // HELIB implements multiplyScalar by changing scale
  multiplyScalar(factor);
}

void HelibCkksCiphertext::setScale(double val)
{
  throw runtime_error("not implemented yet");
}

double HelibCkksCiphertext::getScale() const
{
  throw runtime_error("not implemented yet");
}

void HelibCkksCiphertext::conjugate()
{
  HELAYERS_TIMER_SECTION("HelibCkksCiphertext::conjugate");
  conjugateRaw();
  ctxt.reLinearize();
}

void HelibCkksCiphertext::conjugateRaw()
{
  HELAYERS_TIMER_SECTION("HelibCkksCiphertext::conjugateRaw");
  if (!he.getEnableConjugate()) {
    throw runtime_error("conjugate operation is not enabled");
  }

  ctxt.complexConj();
}

void HelibCkksCiphertext::rotate(int n)
{
  HELAYERS_TIMER("HelibCkksCiphertext::rotate");
  if (he.getMirrored())
    he.getEncryptedArray().rotate(ctxt, n);
  else
    he.getEncryptedArray().rotate(ctxt, -n);
}

void HelibCkksCiphertext::negate()
{
  HELAYERS_TIMER("HelibCkksCiphertext::negate");
  ctxt.negate();
}

int HelibCkksCiphertext::slotCount() const { return he.slotCount(); }
} // namespace helayers
