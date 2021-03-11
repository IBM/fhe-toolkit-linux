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

#include "HeContext.h"
#include "utils/BinIoUtils.h"
#include "AlwaysAssert.h"
#include "impl/AbstractFunctionEvaluator.h"
#include "utils/Saveable.h"
#include <fstream>

using namespace std;

namespace helayers {

HeContext::ContextMap& HeContext::getRegisteredHeContextMap()
{
  static ContextMap contextMap;
  return contextMap;
}

HeContext::HeContext(){};

HeContext::~HeContext(){};

void HeContext::saveToFile(const std::string& fileName, bool withSecretKey)
{
  ofstream out = Saveable::openOfstream(fileName);
  save(out, withSecretKey);
  out.close();
}

void HeContext::loadFromFile(const std::string& fileName)
{
  ifstream in = Saveable::openIfstream(fileName);
  load(in);
  in.close();
}

std::shared_ptr<HeContext> HeContext::clone() const
{
  throw runtime_error("clone not implemented for this context");
}

bool HeContext::internalRegisterContext(const HeContext* context)
{
  string key = context->getContextFileHeaderCode();
  ContextMap& map = getRegisteredHeContextMap();
  if (map.count(key) > 0)
    throw runtime_error("Duplicate context " + key);
  map[key] = context;
  return true;
}

std::string HeContext::getContextFileHeaderCode() const
{
  return getLibraryName() + "_" + getSchemeName();
}

void HeContext::saveSecretKeyToFile(const std::string& fileName)
{
  ofstream out = Saveable::openOfstream(fileName);
  saveSecretKey(out);
  out.close();
}

void HeContext::loadSecretKeyFromFile(const std::string& fileName)
{
  ifstream in = Saveable::openIfstream(fileName);
  loadSecretKey(in);
  in.close();
}

void HeContext::save(std::ostream& out, bool withSecretKey)
{
  BinIoUtils::writeString(out, getContextFileHeaderCode());
  out.write((char*)&defaultScale, sizeof(defaultScale));
}

void HeContext::load(std::istream& in)
{
  string key = BinIoUtils::readString(in);
  if (getContextFileHeaderCode() != key)
    throw runtime_error("Context for " + getContextFileHeaderCode() +
                        " trying to read a context for " + key);
  in.read((char*)&defaultScale, sizeof(defaultScale));
}

shared_ptr<AbstractFunctionEvaluator> HeContext::getFunctionEvaluator()
{
  return make_shared<AbstractFunctionEvaluator>(*this);
}

std::shared_ptr<HeContext> HeContext::loadHeContextFromFile(
    const std::string& fileName)
{
  ifstream in;
  in.open(fileName);
  if (in.fail())
    throw runtime_error("Failed to open file " + fileName);
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::shared_ptr<HeContext> res = loadHeContext(in);
  in.close();
  return res;
}

std::shared_ptr<HeContext> HeContext::loadHeContext(istream& in)
{
  string key = BinIoUtils::readString(in);
  ContextMap& map = getRegisteredHeContextMap();
  ContextMap::iterator it = map.find(key);
  if (it == map.end())
    throw runtime_error("File contains unrecognized context " + key);
  std::shared_ptr<HeContext> res = it->second->clone();
  in.seekg(0);
  res->load(in);
  return res;
}
} // namespace helayers
