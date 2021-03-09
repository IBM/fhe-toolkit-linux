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

#include "TestUtils.h"

using namespace std;
using namespace helayers;

namespace helayerstest {

map<string, HeContext*> TestUtils::hes;

void TestUtils::set(const string& name, HeContext& h)
{
  if (hes.count(name) != 0)
    throw std::runtime_error(name + " already set");
  hes[name] = &h;
}

HeContext& TestUtils::get(const string& name)
{
  if (hes.count(name) != 1)
    throw std::runtime_error(name + " not set");
  return *hes[name];
}

void TestUtils::createOutputDirectory()
{
  FileUtils::createCleanDir(getOutputDirectory());
}

HeContextFactory* TestUtils::heContextFactory = NULL;
double TestUtils::eps = 1e-5;

std::string TestUtils::getOutputDirectory() { return getTestsOutputDir(); }

void TestUtils::printSignatures()
{
  for (auto it = hes.begin(); it != hes.end(); ++it) {
    cout << it->first << endl;
    it->second->printSignature();
  }
}
} // namespace helayerstest
