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

#include "gtest/gtest.h"

#include "hedge/hebase/helib/HelibBgvContext.h"
#include "TestUtils.h"

using namespace hedge;
using namespace hedgetest;
using namespace std;

namespace hedgetest {

class HelibBgvContextFactory : public HeContextFactory
{
public:
  std::shared_ptr<HeContext> create()
  {
    return shared_ptr<HelibBgvContext>(new HelibBgvContext());
  }
};
}

int main(int argc, char** argv)
{
  HelibBgvContextFactory heContextFactory;
  TestUtils::heContextFactory = &heContextFactory;

  // HelibCkksContext temp;
  // temp.init(HelibConfig::createCkks16384());
  // temp.printSignature();
  // exit(0);

  HelibBgvContext he;
  he.init(10009, 4096, 1, 300);
  TestUtils::setHighNumSlots(he);

  TestUtils::setLowNumSlots(he);
  TestUtils::setFastDeepParams(he);

  TestUtils::printSignatures();

  // read --silent flag from cmd args
  bool silentMode = false;
  for (int i = 0; i < argc; ++i) {
    if (std::string(argv[i]) == "--silent")
      silentMode = true;
  }

  // save original cout and cerr
  streambuf* origCout = cout.rdbuf();
  streambuf* origCerr = cerr.rdbuf();

  if (silentMode) {
    // redirect cout and cerr to /dev/null
    ofstream file("/dev/null");
    cout.rdbuf(file.rdbuf());
    cerr.rdbuf(file.rdbuf());
  }

  ::testing::InitGoogleTest(&argc, argv);
  int res = RUN_ALL_TESTS();

  if (silentMode) {
    // restore original cout and cerr
    cout.rdbuf(origCout);
    cerr.rdbuf(origCerr);
  }

  return res;
}