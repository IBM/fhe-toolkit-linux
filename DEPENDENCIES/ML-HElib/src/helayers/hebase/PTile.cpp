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

#include "PTile.h"
#include <fstream>

using namespace std;

namespace helayers {

PTile::PTile(HeContext& he) : impl(he.createAbstractPlain()) {}

PTile::PTile(const PTile& src) : impl(src.impl->clone()) {}

PTile::~PTile() {}

PTile& PTile::operator=(const PTile& src)
{
  if (this != &src)
    impl = src.impl->clone();
  return *this;
}

void PTile::saveToFile(const std::string& fileName) const
{
  ofstream out;
  out.open(fileName, ofstream::out | ofstream::binary);
  if (out.fail())
    throw runtime_error("Failed to open file " + fileName);
  out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  save(out);
  out.close();
}

void PTile::loadFromFile(const std::string& fileName)
{
  ifstream in;
  in.open(fileName);
  if (in.fail())
    throw runtime_error("Failed to open file " + fileName);
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  load(in);
  in.close();
}

streamoff PTile::save(ostream& stream) const { return impl->save(stream); }

streamoff PTile::load(istream& stream) { return impl->load(stream); }

void PTile::reduceChainIndex() { impl->reduceChainIndex(); }

void PTile::setChainIndex(const PTile& other)
{
  impl->setChainIndex(*other.impl);
}

void PTile::setChainIndex(int chainIndex) { impl->setChainIndex(chainIndex); }

int PTile::getChainIndex() const { return impl->getChainIndex(); }

int PTile::slotCount() const { return impl->slotCount(); }

void PTile::debugPrint(const string& title,
                       int maxElements,
                       int verbose,
                       ostream& out) const
{
  return impl->debugPrint(title, maxElements, verbose, out);
}
}
