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

#include <iostream>
#include <fstream>

#include "TestUtils.h"
#include "gtest/gtest.h"
#include "helayers/hebase/hebase.h"

using namespace helayers;
using namespace std;

namespace helayerstest {

TEST(PTileTest, copy)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);
  std::vector<double> v1{3, 3, 3};
  std::vector<double> v2{4, 4, 4};
  std::vector<double> v3{5, 5, 5};
  std::vector<double> vals;

  PTile p1(he);
  enc.encode(p1, v1);

  PTile p2(p1);
  PTile p3 = p1;

  vals = enc.decodeDouble(p1);
  EXPECT_FLOAT_EQ(3, vals[0]);
  vals = enc.decodeDouble(p2);
  EXPECT_FLOAT_EQ(3, vals[0]);
  vals = enc.decodeDouble(p3);
  EXPECT_FLOAT_EQ(3, vals[0]);

  enc.encode(p2, v2);
  enc.encode(p3, v3);

  vals = enc.decodeDouble(p1);
  EXPECT_FLOAT_EQ(3, vals[0]);
  vals = enc.decodeDouble(p2);
  EXPECT_FLOAT_EQ(4, vals[0]);
  vals = enc.decodeDouble(p3);
  EXPECT_FLOAT_EQ(5, vals[0]);
}

TEST(PTileTest, encodeAndDecode)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v{2, 2, 2};
  PTile p(he);
  Encoder enc(he);
  enc.encode(p, v);

  const std::vector<double> vals = enc.decodeDouble(p);
  EXPECT_NEAR(2, vals[0], TestUtils::getEps());
}

TEST(PTileTest, encodeAndDecodeComplex)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsComplexNumbers()) {
    return;
  }

  using namespace std::complex_literals;

  std::vector<complex<double>> v{2. + 2i, 2. + 2i, 2. + 2i};
  PTile p(he);
  Encoder enc(he);
  enc.encode(p, v);

  const std::vector<complex<double>> vals = enc.decodeComplex(p);
  EXPECT_FLOAT_EQ(2, vals[0].real());
  EXPECT_FLOAT_EQ(2, vals[0].imag());
}

TEST(PTileTest, chainIndexMethods)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (he.getTraits().getAutomaticallyManagesChainIndices())
    return;
  std::vector<double> v1{1, 2, 3, 4};
  PTile p(he);
  Encoder enc(he);
  enc.encode(p, v1, he.getTopChainIndex() - 1);

  EXPECT_EQ(p.getChainIndex(), he.getTopChainIndex() - 1);

  enc.encode(p, v1);
  EXPECT_EQ(p.getChainIndex(), he.getTopChainIndex());

  // deprecated
  //  p->setChainIndex(2);
  //  EXPECT_EQ(p->getChainIndex(), 2);
  //
  //  p->reduceChainIndex();
  //  EXPECT_EQ(p->getChainIndex(), 1);

  //  other->encode(v1, 0);
  //  p->setChainIndex(*other);
  //  EXPECT_EQ(p->getChainIndex(), 0);
}

TEST(PTileTest, saveLoad)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount());

  for (double& val : v1) {
    val = ((double)(rand() % 1000)) / 1000;
  }

  PTile p1(he);
  PTile p2(he);
  Encoder enc(he);
  enc.encode(p1, v1);

  TestUtils::createOutputDirectory();
  std::string outFile =
      TestUtils::getOutputDirectory() + "/PTileSaveLoadTest.tmp";
  std::ofstream ofs(outFile, ios::out | ios::binary);
  p1.save(ofs);
  ofs.close();

  std::ifstream ifs(outFile, ios::in | ios::binary);
  p2.load(ifs);
  ifs.close();

  const std::vector<double> vals = enc.decodeDouble(p2);

  EXPECT_EQ(vals.size(), v1.size());

  for (size_t i = 0; i < vals.size(); i++) {
    EXPECT_NEAR(vals[i], v1[i], TestUtils::getEps());
  }
}
}
