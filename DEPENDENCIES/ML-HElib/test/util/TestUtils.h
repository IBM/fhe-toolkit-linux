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

#ifndef TEST_HEDGE_TESTUTILS_H_
#define TEST_HEDGE_TESTUTILS_H_

#include <map>
#include <string>
#include "hedge/hebase/hebase.h"

namespace hedgetest {

class TestUtils
{
  static std::map<std::string, hedge::HeContext*> hes;

  static void set(const std::string& name, hedge::HeContext& h);
  static hedge::HeContext& get(const std::string& name);

  static const std::string outputDirectory;

  static double eps;

public:
  TestUtils();

  static void printSignatures();

  static void setHighNumSlots(hedge::HeContext& h) { set("highNumSlots", h); }
  static hedge::HeContext& getHighNumSlots() { return get("highNumSlots"); }

  static void setLowNumSlots(hedge::HeContext& h) { set("lowNumSlots", h); }
  static hedge::HeContext& getLowNumSlots() { return get("lowNumSlots"); }

  static void setFastDeepParams(hedge::HeContext& h)
  {
    set("fastDeepParams", h);
  }
  static hedge::HeContext& getFastDeepParams() { return get("fastDeepParams"); }

  static void setBootstrapParams(hedge::HeContext& h)
  {
    set("bootstrapParams", h);
  }
  static hedge::HeContext& getBootstrapParams()
  {
    return get("bootstrapParams");
  }

  static std::string getOutputDirectory() { return TestUtils::outputDirectory; }

  static double getEps() { return eps; }
  static void setEps(double e) { eps = e; }

  static void createOutputDirectory();

  class HeContextFactory
  {
  public:
    virtual std::shared_ptr<hedge::HeContext> create() = 0;
  };

  static TestUtils::HeContextFactory* heContextFactory;
};

typedef TestUtils::HeContextFactory HeContextFactory;
}

#endif /* TEST_HEDGE_TESTUTILS_H_ */
