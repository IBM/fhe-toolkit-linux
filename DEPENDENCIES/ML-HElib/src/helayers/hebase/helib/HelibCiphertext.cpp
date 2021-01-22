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
#include "helayers/hebase/impl/AbstractEncoder.h"
#include "helayers/hebase/HelayersTimer.h"

using namespace std;

namespace helayers {

streamoff HelibCiphertext::save(ostream& stream) const
{
  HELAYERS_TIMER("HelibCiphertext::save");
  streampos streamStartPos = stream.tellp();

  ctxt.writeTo(stream);

  streampos streamEndPos = stream.tellp();

  return streamEndPos - streamStartPos;
}

streamoff HelibCiphertext::load(istream& stream)
{
  HELAYERS_TIMER("HelibCiphertext::load");
  streampos streamStartPos = stream.tellg();

  ctxt.read(stream);

  streampos streamEndPos = stream.tellg();

  return streamEndPos - streamStartPos;
}

void HelibCiphertext::add(const AbstractCiphertext& other)
{
  HELAYERS_TIMER_SECTION("HelibCiphertext::add");
  addRaw(other);
}

void HelibCiphertext::addRaw(const AbstractCiphertext& other)
{
  HELAYERS_TIMER("HelibCiphertext::addRaw");
  const HelibCiphertext& castedOther =
      dynamic_cast<const HelibCiphertext&>(other);
  ctxt += castedOther.ctxt;
}

void HelibCiphertext::sub(const AbstractCiphertext& other)
{
  HELAYERS_TIMER_SECTION("HelibCiphertext::sub");
  subRaw(other);
}

void HelibCiphertext::subRaw(const AbstractCiphertext& other)
{
  HELAYERS_TIMER("HelibCiphertext::subRaw");
  const HelibCiphertext& castedOther =
      dynamic_cast<const HelibCiphertext&>(other);
  ctxt -= castedOther.ctxt;
}

void HelibCiphertext::multiply(const AbstractCiphertext& other)
{
  HELAYERS_TIMER("HelibCiphertext::multiply");
  const HelibCiphertext& castedOther =
      dynamic_cast<const HelibCiphertext&>(other);
  ctxt.multiplyBy(castedOther.ctxt);
}

void HelibCiphertext::multiplyRaw(const AbstractCiphertext& other)
{
  HELAYERS_TIMER("HelibCiphertext::multiplyRaw");
  const HelibCiphertext& castedOther =
      dynamic_cast<const HelibCiphertext&>(other);
  ctxt.multLowLvl(castedOther.ctxt);
}

void HelibCiphertext::addPlain(const AbstractPlaintext& plain)
{
  HELAYERS_TIMER_SECTION("HelibCiphertext::addPlain");
  addPlainRaw(plain);
}

void HelibCiphertext::subPlain(const AbstractPlaintext& plain)
{
  HELAYERS_TIMER_SECTION("HelibCiphertext::subPlain");
  subPlainRaw(plain);
}

void HelibCiphertext::multiplyPlain(const AbstractPlaintext& plain)
{
  HELAYERS_TIMER_SECTION("HelibCiphertext::multiplyPlain");
  multiplyPlainRaw(plain);
}

void HelibCiphertext::square()
{
  HELAYERS_TIMER_SECTION("HelibCiphertext::square");
  squareRaw();
}

void HelibCiphertext::squareRaw()
{
  HELAYERS_TIMER("HelibCiphertext::squareRaw");
  multiply(*this);
}

void HelibCiphertext::relinearize()
{
  HELAYERS_TIMER("HelibCiphertext::relinearize");
  ctxt.reLinearize();
}

void HelibCiphertext::reduceChainIndex() {}

void HelibCiphertext::setChainIndex(const AbstractCiphertext& other) {}

void HelibCiphertext::setChainIndex(int chainIndex) {}

void HelibCiphertext::rescale() {}
void HelibCiphertext::rescaleRaw() {}

void HelibCiphertext::setScale(double scale)
{
  throw runtime_error("This method is not supported yet");
}

double HelibCiphertext::getScale() const { return 1; }

int HelibCiphertext::getChainIndex() const { return -1; }

void HelibCiphertext::debugPrint(const string& title,
                                 int maxVals,
                                 int verbose,
                                 ostream& out) const
{
  HELAYERS_TIMER("HelibCiphertext::debugPrint");
  if (title.length() > 0)
    cout << title << endl;
  // TODO: handle const issue
  shared_ptr<AbstractEncoder> encoder = ((HeContext&)getContext()).getEncoder();
  vector<complex<double>> res = encoder->decryptDecodeComplex(*this);
  cout << "slots               : " << res.size() << endl;

  if (maxVals < 0)
    maxVals = res.size();
  out << " ";
  for (int i = 0; i < min(maxVals, (int)res.size()); ++i)
    out << " " << res[i];
  if (maxVals < (res.size() - 1))
    out << " ... " << res[res.size() - 1];
  out << endl;
}
}