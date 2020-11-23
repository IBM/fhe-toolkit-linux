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

TEST(CTileTest, copy)
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

TEST(CTileTest, add)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      v2(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    v2[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] + v1[i] + v2[i];
  }

  CTile src(he);
  CTile c1(he);
  CTile c2(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encodeEncrypt(c1, v1, he.getTopChainIndex());
  enc.encodeEncrypt(c2, v2, he.getTopChainIndex() - 3);

  src.add(c1);
  src.add(c2);

  enc.assertEquals(src, "add", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 3);
}

TEST(CTileTest, addRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] + v1[i];
  }

  CTile src(he);
  CTile c1(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encodeEncrypt(c1, v1, he.getTopChainIndex());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(src.addRaw(c1));
    c1.setChainIndex(src.getChainIndex());
  }

  src.addRaw(c1);

  enc.assertEquals(src, "addRaw", expectedVals, TestUtils::getEps());
}

TEST(CTileTest, sub)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      v2(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    v2[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] - v1[i] - v2[i];
  }

  CTile src(he);
  CTile c1(he);
  CTile c2(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encodeEncrypt(c1, v1, he.getTopChainIndex());
  enc.encodeEncrypt(c2, v2, he.getTopChainIndex() - 3);

  src.sub(c1);
  src.sub(c2);

  enc.assertEquals(src, "sub", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 3);
}

TEST(CTileTest, subRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] - v1[i];
  }

  CTile src(he);
  CTile c1(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encodeEncrypt(c1, v1, he.getTopChainIndex());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(src.subRaw(c1));
    c1.setChainIndex(src.getChainIndex());
  }

  src.subRaw(c1);

  enc.assertEquals(src, "subRaw", expectedVals, TestUtils::getEps());
}

TEST(CTileTest, multiply)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      v2(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    v2[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] * v1[i] * v2[i];
  }

  CTile src(he);
  CTile c1(he);
  CTile c2(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex());
  enc.encodeEncrypt(c1, v1, he.getTopChainIndex() - 1);
  enc.encodeEncrypt(c2, v2, he.getTopChainIndex());

  double origScale;
  if (he.getTraits().getSupportsExplicitRescale())
    origScale = src.getScale();

  src.multiply(c1);
  src.multiply(c2);

  enc.assertEquals(src, "multiply", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 3);
  if (he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(src.getScale(), origScale);
}

TEST(CTileTest, multiplyRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] * v1[i];
  }

  CTile src(he);
  CTile c1(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encodeEncrypt(c1, v1, he.getTopChainIndex());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(src.multiplyRaw(c1));
    c1.setChainIndex(src.getChainIndex());
  }

  double origScale;
  if (he.getTraits().getSupportsExplicitRescale())
    origScale = src.getScale();

  src.multiplyRaw(c1);

  enc.assertEquals(src, "multiplyRaw", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 1);
  if (he.getTraits().getSupportsExplicitRescale())
    EXPECT_NE(src.getScale(), origScale);
}

TEST(CTileTest, addPlain)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      v2(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    v2[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] + v1[i] + v2[i];
  }

  CTile src(he);
  PTile p1(he);
  PTile p2(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encode(p1, v1, he.getTopChainIndex());
  enc.encode(p2, v2, he.getTopChainIndex() - 3);

  src.addPlain(p1);
  src.addPlain(p2);

  enc.assertEquals(src, "addPlain", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 3);
}

TEST(CTileTest, addPlainRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] + v1[i];
  }

  CTile src(he);
  PTile p1(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encode(p1, v1, he.getTopChainIndex());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(src.addPlainRaw(p1));
    p1.setChainIndex(src.getChainIndex());
  }

  src.addPlainRaw(p1);

  enc.assertEquals(src, "addPlainRaw", expectedVals, TestUtils::getEps());
}

TEST(CTileTest, subPlain)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      v2(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    v2[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] - v1[i] - v2[i];
  }

  CTile src(he);
  PTile p1(he);
  PTile p2(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encode(p1, v1, he.getTopChainIndex());
  enc.encode(p2, v2, he.getTopChainIndex() - 3);

  src.subPlain(p1);
  src.subPlain(p2);

  enc.assertEquals(src, "subPlain", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 3);
}

TEST(CTileTest, subPlainRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] - v1[i];
  }

  CTile src(he);
  PTile p1(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encode(p1, v1, he.getTopChainIndex());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(src.subPlainRaw(p1));
    p1.setChainIndex(src.getChainIndex());
  }

  src.subPlainRaw(p1);

  enc.assertEquals(src, "subPlainRaw", expectedVals, TestUtils::getEps());
}

TEST(CTileTest, multiplyPlain)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      v2(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    v2[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] * v1[i] * v2[i];
  }

  CTile src(he);
  PTile p1(he);
  PTile p2(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex());
  enc.encode(p1, v1, he.getTopChainIndex() - 1);
  enc.encode(p2, v2, he.getTopChainIndex());

  double origScale;
  if (he.getTraits().getSupportsExplicitRescale())
    origScale = src.getScale();

  src.multiplyPlain(p1);
  src.multiplyPlain(p2);

  enc.assertEquals(src, "multiplyPlain", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 3);
  if (he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(src.getScale(), origScale);
}

TEST(CTileTest, multiplyPlainRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> vSrc(he.slotCount()), v1(he.slotCount()),
      expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    vSrc[i] = ((double)(rand() % 1000)) / 1000;
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = vSrc[i] * v1[i];
  }

  CTile src(he);
  PTile p1(he);

  enc.encodeEncrypt(src, vSrc, he.getTopChainIndex() - 1);
  enc.encode(p1, v1, he.getTopChainIndex());

  if (!he.getTraits().getAutomaticallyManagesChainIndices()) {
    EXPECT_ANY_THROW(src.multiplyPlainRaw(p1));
    p1.setChainIndex(src.getChainIndex());
  }

  double origScale;
  if (he.getTraits().getSupportsExplicitRescale())
    origScale = src.getScale();

  src.multiplyPlainRaw(p1);

  enc.assertEquals(src, "multiplyPlainRaw", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(src.getChainIndex(), he.getTopChainIndex() - 1);
  if (he.getTraits().getSupportsExplicitRescale())
    EXPECT_NE(src.getScale(), origScale);
}

TEST(CTileTest, square)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = v1[i] * v1[i];
  }

  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  double origScale;
  if (he.getTraits().getSupportsExplicitRescale())
    origScale = c1.getScale();

  c1.square();

  enc.assertEquals(c1, "square", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(c1.getChainIndex(), he.getTopChainIndex() - 1);
  if (he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(c1.getScale(), origScale);
}

TEST(CTileTest, squareRaw)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1(he.slotCount()), expectedVals(he.slotCount());

  for (int i = 0; i < he.slotCount(); i++) {
    v1[i] = ((double)(rand() % 1000)) / 1000;
    expectedVals[i] = v1[i] * v1[i];
  }

  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  double origScale;
  if (he.getTraits().getSupportsExplicitRescale())
    origScale = c1.getScale();

  c1.squareRaw();

  enc.assertEquals(c1, "squareRaw", expectedVals, TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(c1.getChainIndex(), he.getTopChainIndex());
  if (he.getTraits().getSupportsExplicitRescale())
    EXPECT_NE(c1.getScale(), origScale);
}

TEST(CTileTest, addScalarInt)
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

TEST(CTileTest, addScalarDouble)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  double scalar = 3.5;
  int chainIndex = c1.getChainIndex();

  c1.addScalar(scalar);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(5.5, vals[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices() &&
      he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(chainIndex, c1.getChainIndex());

  c1.multiply(c2);
  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(16.5, vals2[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices() &&
      he.getTraits().getSupportsExplicitRescale())
    EXPECT_EQ(chainIndex - 1, c1.getChainIndex());
}

TEST(CTileTest, multiplyScalarInt)
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

TEST(CTileTest, multiplyScalarDouble)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);

  double scalar = 3.5;
  int chainIndex = c1.getChainIndex();

  c1.multiplyScalar(scalar);
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(7, vals[0], TestUtils::getEps());
  if (he.getTraits().getSupportsExplicitChainIndices()) {
    EXPECT_EQ(chainIndex - 1, c1.getChainIndex());
    c2.setChainIndex(c1);
  }

  c1.multiply(c2);
  const std::vector<double> vals2 = enc.decryptDecodeDouble(c1);

  EXPECT_NEAR(21, vals2[0], 1e-4);
  if (he.getTraits().getSupportsExplicitChainIndices())
    EXPECT_EQ(chainIndex - 2, c1.getChainIndex());
}

TEST(CTileTest, rotate)
{
  HeContext& he = TestUtils::getHighNumSlots();

  Encoder enc(he);
  std::vector<double> v1;
  for (int i = 0; i < he.slotCount(); ++i)
    v1.push_back(0.1 + i * 0.1);
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

TEST(CTileTest, encrypt)
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

TEST(CTileTest, negate)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{1, -2, 3, -4};
  std::vector<double> v1Neg{-1, 2, -3, 4};

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  c1.negate();

  double maxDiff = enc.assertEquals(c1, "test1", v1Neg);
  EXPECT_LT(maxDiff, 1e-6);

  cout << "Negative test below, expecting an error to be printed" << endl;
  EXPECT_ANY_THROW(enc.assertEquals(c1, "test2", v1));
}

TEST(CTileTest, getAndSetScale)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsExplicitRescale())
    return;
  std::vector<double> v1{2, 3, 4, 5};
  std::vector<double> v1Half{1, 1.5, 2, 2.5};

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  double scale = c1.getScale();
  c1.setScale(scale * 2);

  double maxDiff = enc.assertEquals(c1, "test division", v1Half);
  EXPECT_LT(maxDiff, 1e-6);

  c1.multiplyByChangingScale(2);
  c1.debugPrint();
  maxDiff = enc.assertEquals(c1, "test multiplication", v1);
  EXPECT_LT(maxDiff, 1e-6);
}

TEST(CTileTest, innerSum)
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

TEST(CTileTest, sumExpBySquaringRightToLeft)
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

TEST(CTileTest, sumExpBySquaringLeftToRight)
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

TEST(CTileTest, operationsWithComplex)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsComplexNumbers()) {
    return;
  }

  using namespace std::complex_literals;

  std::vector<complex<double>> v1{1. + 1i, 1. + 1i, 1. + 1i, 1. + 1i};
  std::vector<complex<double>> v2{2. - 3i, 2. - 3i, 2. - 3i, 2. - 3i};
  std::vector<complex<double>> v3{4. - 1i, 4. - 1i, 4. - 1i, 4. - 1i};
  std::vector<complex<double>> v4{3. + 4i, 3. + 4i, 3. + 4i, 3. + 4i};

  CTile c1(he);
  CTile c2(he);
  CTile c3(he);
  CTile c4(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);
  enc.encodeEncrypt(c2, v2);
  enc.encodeEncrypt(c3, v3);
  enc.encodeEncrypt(c4, v4);

  c1.add(c2); // 3-2i
  const std::vector<complex<double>> vals1 = enc.decryptDecodeComplex(c1);
  EXPECT_FLOAT_EQ(3, vals1[0].real());
  EXPECT_FLOAT_EQ(-2, vals1[0].imag());

  c1.sub(c3); // -1-i
  const std::vector<complex<double>> vals2 = enc.decryptDecodeComplex(c1);

  EXPECT_FLOAT_EQ(-1, vals2[0].real());
  EXPECT_FLOAT_EQ(-1, vals2[0].imag());

  c2.multiply(c3); // 5-14i
  const std::vector<complex<double>> vals3 = enc.decryptDecodeComplex(c2);
  EXPECT_FLOAT_EQ(5, vals3[0].real());
  EXPECT_FLOAT_EQ(-14, vals3[0].imag());

  c3.square(); // 15-8i
  const std::vector<complex<double>> vals4 = enc.decryptDecodeComplex(c3);
  EXPECT_FLOAT_EQ(15, vals4[0].real());
  EXPECT_FLOAT_EQ(-8, vals4[0].imag());

  c4.innerSum(1, 4);
  const std::vector<complex<double>> vals5 = enc.decryptDecodeComplex(c4);
  EXPECT_FLOAT_EQ(12, vals5[0].real());
  EXPECT_FLOAT_EQ(16, vals5[0].imag());
  EXPECT_FLOAT_EQ(9, vals5[1].real());
  EXPECT_FLOAT_EQ(12, vals5[1].imag());
  EXPECT_FLOAT_EQ(6, vals5[2].real());
  EXPECT_FLOAT_EQ(8, vals5[2].imag());
  EXPECT_FLOAT_EQ(3, vals5[3].real());
  EXPECT_FLOAT_EQ(4, vals5[3].imag());
  EXPECT_NEAR(0, vals5[4].real(), TestUtils::getEps());
  EXPECT_NEAR(0, vals5[4].imag(), TestUtils::getEps());

  c4.conjugate(); // 12-16i
  const std::vector<complex<double>> vals6 = enc.decryptDecodeComplex(c4);
  EXPECT_FLOAT_EQ(12, vals6[0].real());
  EXPECT_FLOAT_EQ(-16, vals6[0].imag());
}

TEST(CTileTest, highScaleMultiply)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (he.getTraits().getSupportsBitwiseOperations()) {
    // High scales are currently not supported for bitwise ciphertexts
    return;
  }

  double orgScale = he.getDefaultScale();
  he.setDefaultScale(pow(2, 80));
  std::vector<double> v1{2, 2, 2};
  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);
  c1.debugPrint("c1");

  std::vector<double> v2{3, 3, 3};
  CTile c2(he);
  enc.encodeEncrypt(c2, v2);
  c2.debugPrint("c2");

  c1.multiply(c2);
  c1.debugPrint("c1");
  const std::vector<double> vals = enc.decryptDecodeDouble(c1);

  EXPECT_FLOAT_EQ(6, vals[0]);

  he.setDefaultScale(orgScale);
}
TEST(CTileTest, saveLoad)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1(he.slotCount());

  for (double& val : v1) {
    val = ((double)(rand() % 1000)) / 1000;
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
