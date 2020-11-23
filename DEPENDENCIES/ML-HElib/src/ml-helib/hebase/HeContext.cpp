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

#include "HeContext.h"
#include "impl/AbstractFunctionEvaluator.h"
#include <fstream>

using namespace std;

namespace hedge {

HeContext::HeContext(){};

HeContext::~HeContext(){};

void HeContext::saveToFile(const std::string& fileName, bool withSecretKey)
{
  ofstream out;
  out.open(fileName, ofstream::out | ofstream::binary);
  if (out.fail())
    throw runtime_error("Failed to open file " + fileName);
  out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  out.write((char*)&defaultScale, sizeof(defaultScale));
  save(out, withSecretKey);
  out.close();
}

void HeContext::loadFromFile(const std::string& fileName)
{
  ifstream in;
  in.open(fileName);
  if (in.fail())
    throw runtime_error("Failed to open file " + fileName);
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  in.read((char*)&defaultScale, sizeof(defaultScale));
  load(in);
  in.close();
}

void HeContext::saveSecretKeyToFile(const std::string& fileName)
{
  ofstream out;
  out.open(fileName, ofstream::out | ofstream::binary);
  if (out.fail())
    throw runtime_error("Failed to open file " + fileName);
  out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  saveSecretKey(out);
  out.close();
}

void HeContext::loadSecretKeyFromFile(const std::string& fileName)
{
  ifstream in;
  in.open(fileName);
  if (in.fail())
    throw runtime_error("Failed to open file " + fileName);
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  loadSecretKey(in);
  in.close();
}

void HeContext::save(std::ostream& out, bool withSecretKey)
{
  out.write((char*)&defaultScale, sizeof(defaultScale));
}

void HeContext::load(std::istream& in)
{
  in.read((char*)&defaultScale, sizeof(defaultScale));
}

shared_ptr<AbstractFunctionEvaluator> HeContext::getFunctionEvaluator()
{
  return make_shared<AbstractFunctionEvaluator>(*this);
}
}
