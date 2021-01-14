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

#include "CTile.h"
#include <fstream>

using namespace std;

namespace helayers {

CTile::CTile(HeContext& he) : impl(he.createAbstractCipher()) {}

CTile::CTile(const CTile& src) : impl(src.impl->clone()) {}

CTile::~CTile() {}

CTile& CTile::operator=(const CTile& src)
{
  if (this != &src)
    impl = src.impl->clone();
  return *this;
}

void CTile::saveToFile(const std::string& fileName) const
{
  ofstream out;
  out.open(fileName, ofstream::out | ofstream::binary);
  if (out.fail())
    throw runtime_error("Failed to open file " + fileName);
  out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  save(out);
  out.close();
}

void CTile::loadFromFile(const std::string& fileName)
{
  ifstream in;
  in.open(fileName);
  if (in.fail())
    throw runtime_error("Failed to open file " + fileName);
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  load(in);
  in.close();
}

streamoff CTile::save(ostream& stream) const { return impl->save(stream); }

streamoff CTile::load(istream& stream) { return impl->load(stream); }

void CTile::conjugate() { impl->conjugate(); }

void CTile::conjugateRaw() { impl->conjugateRaw(); }

void CTile::rotate(int n) { impl->rotate(n); }

void CTile::innerSum(int rot1, int rot2, bool reverse)
{
  impl->innerSum(rot1, rot2, reverse);
}

void CTile::sumExpBySquaringLeftToRight(int n)
{
  impl->sumExpBySquaringLeftToRight(n);
}

void CTile::sumExpBySquaringRightToLeft(int n)
{
  impl->sumExpBySquaringRightToLeft(n);
}

void CTile::add(const CTile& other) { impl->add(*other.impl); }

void CTile::addRaw(const CTile& other) { impl->addRaw(*other.impl); }

void CTile::sub(const CTile& other) { impl->sub(*other.impl); }

void CTile::subRaw(const CTile& other) { impl->subRaw(*other.impl); }

void CTile::multiply(const CTile& other) { impl->multiply(*other.impl); }

void CTile::multiplyRaw(const CTile& other) { impl->multiplyRaw(*other.impl); }

void CTile::addPlain(const PTile& plain) { impl->addPlain(*plain.impl); }

void CTile::addPlainRaw(const PTile& plain) { impl->addPlainRaw(*plain.impl); }

void CTile::subPlain(const PTile& plain) { impl->subPlain(*plain.impl); }

void CTile::subPlainRaw(const PTile& plain) { impl->subPlainRaw(*plain.impl); }

void CTile::multiplyPlain(const PTile& plain)
{
  impl->multiplyPlain(*plain.impl);
}

void CTile::multiplyPlainRaw(const PTile& plain)
{
  impl->multiplyPlainRaw(*plain.impl);
}

void CTile::square() { impl->square(); }

void CTile::squareRaw() { impl->squareRaw(); }

void CTile::addScalar(int scalar) { impl->addScalar(scalar); }

void CTile::addScalar(double scalar) { impl->addScalar(scalar); }

void CTile::multiplyScalar(int scalar) { impl->multiplyScalar(scalar); }

void CTile::multiplyScalar(double scalar) { impl->multiplyScalar(scalar); }

void CTile::negate() { impl->negate(); }

void CTile::multiplyByChangingScale(double factor)
{
  impl->multiplyByChangingScale(factor);
}

void CTile::setScale(double scale) { impl->setScale(scale); }

double CTile::getScale() const { return impl->getScale(); }

void CTile::relinearize() { impl->relinearize(); }

void CTile::rescale() { impl->rescale(); }

void CTile::rescaleRaw() { impl->rescaleRaw(); }

void CTile::reduceChainIndex() { impl->reduceChainIndex(); }

void CTile::setChainIndex(const CTile& other)
{
  impl->setChainIndex(*other.impl);
}

void CTile::setChainIndex(int chainIndex) { impl->setChainIndex(chainIndex); }

int CTile::getChainIndex() const { return impl->getChainIndex(); }

int CTile::slotCount() const { return impl->slotCount(); }

bool CTile::isEmpty() const { return impl->isEmpty(); }

void CTile::debugPrint(const string& title,
                       int maxElements,
                       int verbose,
                       ostream& out) const
{
  cout << "===========================================================" << endl;
  return impl->debugPrint(title, maxElements, verbose, out);
}
}
