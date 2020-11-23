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
#include "hedge/hebase/hebase.h"
#include "TestUtils.h"
#include "gtest/gtest.h"

using namespace std;
using namespace hedge;

namespace hedgetest {

TEST(CTileIntTest, copy)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);
  std::vector<double> v1{3, 3, 3};
  std::vector<double> v2{4, 4, 4};
  std::vector<double> v3{5, 5, 5};
  std::vector<double> vals;

  CTile c1(he);

  enc.encodeEncrypt(c1, v1);

  CTile c2(c1);
  CTile c3 = c1;

  vals = enc.decryptDecodeDouble(c1);
  EXPECT_NEAR(3, vals[0], TestUtils::getEps());
  vals = enc.decryptDecodeDouble(c2);
  EXPECT_NEAR(3, vals[0], TestUtils::getEps());
  vals = enc.decryptDecodeDouble(c3);
  EXPECT_NEAR(3, vals[0], TestUtils::getEps());

  enc.encodeEncrypt(c2, v2);
  enc.encodeEncrypt(c3, v3);

  vals = enc.decryptDecodeDouble(c1);
  EXPECT_NEAR(3, vals[0], TestUtils::getEps());
  vals = enc.decryptDecodeDouble(c2);
  EXPECT_NEAR(4, vals[0], TestUtils::getEps());
  vals = enc.decryptDecodeDouble(c3);
  EXPECT_NEAR(5, vals[0], TestUtils::getEps());

  c1.square();

  vals = enc.decryptDecodeDouble(c1);
  EXPECT_NEAR(9, vals[0], TestUtils::getEps());
  vals = enc.decryptDecodeDouble(c2);
  EXPECT_NEAR(4, vals[0], TestUtils::getEps());
  vals = enc.decryptDecodeDouble(c3);
  EXPECT_NEAR(5, vals[0], TestUtils::getEps());
}

TEST(CTileIntTest, add)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  c1.add(c2);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_FLOAT_EQ(5, vals[0]);
}

TEST(CTileIntTest, sub)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{5, 5, 5};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  c1.sub(c2);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_FLOAT_EQ(2, vals[0]);
}

TEST(CTileIntTest, multiply)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  c1.multiply(c2);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(6, vals[0], TestUtils::getEps());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(c1.multiply(c2));
  }

  c2.setChainIndex(c1);
  c1.multiply(c2);
  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(1, 19 / (1 + vals2[0]), TestUtils::getEps());
}

TEST(CTileIntTest, multiplyPlain)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount()), v2(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    v1[i] = ((double)(rand() % 10));
    v2[i] = ((double)(rand() % 10));
    expectedVals[i] = v1[i] * v2[i];
  }

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);
  enc.encode(p, v2, c1.getChainIndex());

  c1.multiplyPlain(p);
  enc.assertEquals(c1, "multiplyPlain", expectedVals, TestUtils::getEps());
}

TEST(CTileIntTest, power)
{
  HeContext& he = TestUtils::getHighNumSlots();
  CTile c(he);
  Encoder enc(he);
  NativeFunctionEvaluator eval(he);

  std::vector<double> v1{1, 2, 3}, expectedVals1{1, 4, 9};
  int exponent = 2;
  enc.encodeEncrypt(c, v1);
  eval.powerInPlace(c, exponent);
  enc.assertEquals(c, "power", expectedVals1, TestUtils::getEps());

  std::vector<double> v2{1, 2, 3}, expectedVals2{1, 128, 2187};
  exponent = 7;
  enc.encodeEncrypt(c, v2);
  eval.powerInPlace(c, exponent);
  enc.assertEquals(c, "power", expectedVals2, TestUtils::getEps());
}

TEST(CTileIntTest, totalProduct)
{
  HeContext& he = TestUtils::getHighNumSlots();
  CTile c1(he), c2(he), c3(he);
  CTile result(he);
  Encoder enc(he);
  NativeFunctionEvaluator eval(he);
  enc.encodeEncrypt(c1, std::vector<double>{1});
  enc.encodeEncrypt(c2, std::vector<double>{2});
  enc.encodeEncrypt(c3, std::vector<double>{3});

  std::vector<CTile> cipherVals{c1, c2, c3};
  std::vector<double> expectedVal = {6}; 
  eval.totalProduct(result, cipherVals);
  enc.assertEquals(result, "totalProduct", expectedVal, TestUtils::getEps());
}

TEST(CTileIntTest, multiplyPlainRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount()), v2(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    v1[i] = ((double)(rand() % 10));
    v2[i] = ((double)(rand() % 10));
    expectedVals[i] = v1[i] * v2[i];
  }

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);
  enc.encode(p, v2, c1.getChainIndex());

  double scale = c1.getScale();
  int orgChainIndex = c1.getChainIndex();
  c1.multiplyPlainRaw(p);
  enc.assertEquals(c1, "multiplyPlainRaw", expectedVals);
  if (he.getTraits().getSupportsExplicitRescale()) {
    assert(c1.getChainIndex() == orgChainIndex);
    assert(c1.getScale() != scale);
  }
}

TEST(CTileIntTest, addScalarInt)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  int scalar = 4;
  int chainIndex = c1.getChainIndex();

  c1.addScalar(scalar);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(6, vals[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices() &&
      he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(chainIndex, c1.getChainIndex());

  c1.multiply(c2);
  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(18, vals2[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices() &&
      he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(chainIndex - 1, c1.getChainIndex());
}

TEST(CTileIntTest, multiplyScalarInt)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  int scalar = 4;
  int chainIndex = c1.getChainIndex();

  c1.multiplyScalar(scalar);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(8, vals[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices() &&
      he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(chainIndex, c1.getChainIndex());

  c1.multiply(c2);
  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(24, vals2[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices() &&
      he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(chainIndex - 1, c1.getChainIndex());
}

TEST(CTileIntTest, addPlain)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount()), v2(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    v1[i] = ((double)(rand() % 10));
    v2[i] = ((double)(rand() % 10));
    expectedVals[i] = v1[i] + v2[i];
  }

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);
  enc.encode(p, v2, c1.getChainIndex());

  c1.addPlain(p);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  enc.assertEquals(c1, "addPlain", expectedVals, TestUtils::getEps());
}

TEST(CTileIntTest, subPlain)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount()), v2(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    v1[i] = ((double)(rand() % 10)) + 20;
    v2[i] = ((double)(rand() % 10));
    expectedVals[i] = v1[i] - v2[i];
  }

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);
  enc.encode(p, v2, c1.getChainIndex());

  c1.subPlain(p);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  enc.assertEquals(c1, "subPlain", expectedVals, TestUtils::getEps());
}

TEST(CTileIntTest, rotate)
{
  HeContext& he = TestUtils::getHighNumSlots();

  Encoder enc(he);
  std::vector<double> v1;
  for (int i = 0; i < he.slotCount(); ++i)
    v1.push_back(i);
  int rots[] = {1, -1, 0, 5, -7};
  for (int rot : rots) {
    CTile c1(he);
    enc.encodeEncrypt(c1, v1);

    c1.rotate(rot);

    const std::vector<double> vals = enc.decryptDecodeDouble(c1);
    for (size_t i = 0; i < v1.size(); ++i) {
      int rotInd = i - rot;
      while (rotInd < 0) {
        rotInd += he.slotCount();
      }
      rotInd %= he.slotCount();

      EXPECT_NEAR(v1[i], vals[rotInd], TestUtils::getEps());
    }
  }
}

TEST(CTileIntTest, square)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{1, 2, 3, 4};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  c1.square();

  const std::vector<double> vals = enc.decryptDecodeDouble(c1);
  EXPECT_NEAR(1, vals[0], TestUtils::getEps());
  EXPECT_NEAR(4, vals[1], TestUtils::getEps());
  EXPECT_NEAR(9, vals[2], TestUtils::getEps());
  EXPECT_NEAR(16, vals[3], TestUtils::getEps());
  EXPECT_NEAR(0, vals[4], TestUtils::getEps());
}

TEST(CTileIntTest, encrypt)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);
  std::vector<double> v2{3, 3, 3};
  enc.encode(p, v2);
  CTile c2(he);
  enc.encrypt(c2, p);

  c1.multiply(c2);
  const std::vector<double> vals1 = enc.decryptDecodeDouble(c1);
  EXPECT_NEAR(6, vals1[0], TestUtils::getEps());

  CTile c3(he);
  enc.encodeEncrypt(c3, v1, c1.getChainIndex());

  c1.multiply(c3);
  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);
  EXPECT_NEAR(12, vals2[0], TestUtils::getEps());
}

TEST(CTileIntTest, innerSum)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{1, 2, 3, 4};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  c1.innerSum(1, 4);

  const std::vector<double> vals = enc.decryptDecodeDouble(c1);
  EXPECT_FLOAT_EQ(1 + 2 + 3 + 4, vals[0]);
  int expectedVal1 = 2 + 3 + 4;
  int expectedVal2 = 3 + 4;
  int expectedVal3 = 4;
  if (he.slotCount() == 4) {
    expectedVal1 = expectedVal2 = expectedVal3 = 1 + 2 + 3 + 4;
  }
  EXPECT_FLOAT_EQ(expectedVal1, vals[1]);
  EXPECT_FLOAT_EQ(expectedVal2, vals[2]);
  EXPECT_FLOAT_EQ(expectedVal3, vals[3]);
  if (he.slotCount() >= 8) {
    EXPECT_NEAR(0, vals[4], TestUtils::getEps());
  }
}

TEST(CTileIntTest, sumExpBySquaringRightToLeft)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v{1, 2, 3, 4};
  CTile c(he);

  Encoder enc(he);
  enc.encodeEncrypt(c, v);

  CTile c1(c);

  c1.sumExpBySquaringRightToLeft(4);

  const std::vector<double> vals1 = enc.decryptDecodeDouble(c1);
  EXPECT_FLOAT_EQ(1 + 2 + 3 + 4, vals1[0]);
  int expectedSum1 = 2 + 3 + 4;
  int expectedSum2 = 3 + 4;
  int expectedSum3 = 4;
  if (he.slotCount() == 4) {
    expectedSum1 = expectedSum2 = expectedSum3 = 1 + 2 + 3 + 4;
  }
  EXPECT_FLOAT_EQ(expectedSum1, vals1[1]);
  EXPECT_FLOAT_EQ(expectedSum2, vals1[2]);
  EXPECT_FLOAT_EQ(expectedSum3, vals1[3]);
  EXPECT_NEAR(0, vals1[4], TestUtils::getEps());

  c1 = c;
  c1.sumExpBySquaringRightToLeft(3);

  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);
  EXPECT_FLOAT_EQ(1 + 2 + 3, vals2[0]);
  EXPECT_FLOAT_EQ(2 + 3 + 4, vals2[1]);
  expectedSum2 = 3 + 4;
  expectedSum3 = 4;
  if (he.slotCount() == 4) {
    expectedSum2 += 1;
    expectedSum3 += (1 + 2);
  }
  EXPECT_FLOAT_EQ(expectedSum2, vals2[2]);
  EXPECT_FLOAT_EQ(expectedSum3, vals2[3]);
  if (he.slotCount() >= 7) {
    EXPECT_NEAR(0, vals2[4], TestUtils::getEps());
  }
}

TEST(CTileIntTest, sumExpBySquaringLeftToRight)
{
  HeContext& he = TestUtils::getHighNumSlots();
  std::vector<double> v{1, 2, 3, 4};
  CTile c(he);

  Encoder enc(he);
  enc.encodeEncrypt(c, v);

  CTile c1(c);

  c1.sumExpBySquaringLeftToRight(4);

  const std::vector<double> vals1 = enc.decryptDecodeDouble(c1);
  EXPECT_FLOAT_EQ(1 + 2 + 3 + 4, vals1[0]);
  int expectedSum1 = 2 + 3 + 4;
  int expectedSum2 = 3 + 4;
  int expectedSum3 = 4;
  if (he.slotCount() == 4) {
    expectedSum1 = expectedSum2 = expectedSum3 = 1 + 2 + 3 + 4;
  }
  EXPECT_FLOAT_EQ(expectedSum1, vals1[1]);
  EXPECT_FLOAT_EQ(expectedSum2, vals1[2]);
  EXPECT_FLOAT_EQ(expectedSum3, vals1[3]);
  EXPECT_NEAR(0, vals1[4], TestUtils::getEps());

  c1 = c;
  c1.sumExpBySquaringLeftToRight(3);

  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);
  EXPECT_FLOAT_EQ(1 + 2 + 3, vals2[0]);
  EXPECT_FLOAT_EQ(2 + 3 + 4, vals2[1]);
  expectedSum2 = 3 + 4;
  expectedSum3 = 4;
  if (he.slotCount() == 4) {
    expectedSum2 += 1;
    expectedSum3 += (1 + 2);
  }
  EXPECT_FLOAT_EQ(expectedSum2, vals2[2]);
  EXPECT_FLOAT_EQ(expectedSum3, vals2[3]);
  if (he.slotCount() >= 7) {
    EXPECT_NEAR(0, vals2[4], TestUtils::getEps());
  }
}

TEST(CTileIntTest, saveLoad)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount());

  for (double& val : v1) {
    val = ((double)(rand() % 10));
  }

  CTile c1(he);
  CTile c2(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);
  c1.debugPrint("c1");

  TestUtils::createOutputDirectory();
  std::string outFile =
      TestUtils::getOutputDirectory() + "/CTileSaveLoadTest.tmp";
  std::ofstream ofs(outFile, ios::out | ios::binary);
  c1.save(ofs);
  ofs.close();

  std::ifstream ifs(outFile, ios::in | ios::binary);
  c2.load(ifs);
  ifs.close();
  c2.debugPrint("c2");

  enc.assertEquals(c2, "saveLoadTest", v1, TestUtils::getEps());
}
} // namespace hedgetest