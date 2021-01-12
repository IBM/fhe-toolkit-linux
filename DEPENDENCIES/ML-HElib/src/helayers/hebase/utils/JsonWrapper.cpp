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

#include "JsonWrapper.h"
#include "JsonSubtree.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

namespace helayers {

JsonWrapper::~JsonWrapper() { clear(); }

void JsonWrapper::load(const char* inBuf, int len)
{
  string str(inBuf, inBuf + len);
  load(str);
}

void JsonWrapper::load(const std::string& in)
{
  stringstream str(in);
  load(str);
}

void JsonWrapper::load(std::istream& in)
{
  clear();
  pt = new boost::property_tree::ptree();
  boost::property_tree::read_json(in, *pt);
}

void JsonWrapper::clear()
{
  if (pt != NULL) {
    delete pt;
    pt = NULL;
  }
}
void JsonWrapper::print(ostream& out, bool pretty) const
{
  assertInitialized();
  boost::property_tree::write_json(out, *pt, pretty);
}

string JsonWrapper::getString(const std::string& key) const
{
  assertInitialized();
  return pt->get<string>(key);
}

int JsonWrapper::getInt(const std::string& key) const
{
  assertInitialized();
  return pt->get<int>(key);
}

double JsonWrapper::getDouble(const std::string& key) const
{
  assertInitialized();
  return pt->get<double>(key);
}

std::ostream& operator<<(ostream& out, const JsonWrapper& jw)
{
  jw.print(out, true);
  return out;
}

string JsonWrapper::toString() const
{
  assertInitialized();
  stringstream out;
  print(out, false);
  return out.str();
}

void JsonWrapper::loadFromBinary(std::istream& in)
{
  int sz;
  in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
  char buff[sz];
  in.read(buff, sizeof(char) * sz);
  string data(buff);
  load(data);
}

void JsonWrapper::writeToBinary(std::ostream& out) const
{
  assertInitialized();
  string data = toString();
  int sz = data.size() + 1;
  out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
  out.write(data.c_str(), sizeof(char) * sz);
}

bool JsonWrapper::isInitialized() const { return pt != NULL; }

void JsonWrapper::assertInitialized() const
{
  if (!isInitialized())
    throw runtime_error("This JsonWrapper object is not initialized");
}

JsonSubtree JsonWrapper::getRoot() const
{
  assertInitialized();
  return JsonSubtree(pt);
}
}