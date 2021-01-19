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
#include <chrono>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "helayers/hebase/hebase.h"
#include "TestUtils.h"

using namespace std;
using namespace std::chrono;
using namespace helayers;

namespace helayerstest {

static void verifyDecode(const PTile& p,
                         vector<complex<double>> expected,
                         const Encoder& enc)
{
  vector<complex<double>> complexRes = enc.decodeComplex(p);
  vector<double> doubleRes = enc.decodeDouble(p);
  vector<int> intRes = enc.decodeInt(p);
  vector<long> longRes = enc.decodeLong(p);

  for (size_t i = 0; i < expected.size(); i++) {
    EXPECT_NEAR(complexRes[i].real(), expected[i].real(), TestUtils::getEps());
    EXPECT_NEAR(complexRes[i].imag(), expected[i].imag(), TestUtils::getEps());
    EXPECT_NEAR(doubleRes[i], expected[i].real(), TestUtils::getEps());
    EXPECT_EQ(intRes[i], lround(complexRes[i].real()));
    EXPECT_EQ(longRes[i], lround(complexRes[i].real()));
  }
}

TEST(EncoderTest, encodeComplex)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsComplexNumbers()) {
    return;
  }

  Encoder enc(he);

  std::vector<complex<double>> v{2. + 2i, 3.4 + 2.2i, -2.003 + 0.003i};

  PTile p(he);
  enc.encode(p, v);

  verifyDecode(p, v, enc);
}

TEST(EncoderTest, encodeDoubles)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v{2.51, 3.2, -5.3};

  std::vector<complex<double>> vComplex{v[0], v[1], v[2]};

  PTile p(he);
  enc.encode(p, v);

  verifyDecode(p, vComplex, enc);
}

TEST(EncoderTest, encodeIntegers)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  vector<int> vInt{2, 3, -5};
  vector<long> vLong{2, 3, -5};
  std::vector<complex<double>> vComplex(3);
  for (size_t i = 0; i < vComplex.size(); i++) {
    vComplex[i] = static_cast<complex<double>>(vInt[i]);
  }

  PTile pInt(he), pLong(he);
  enc.encode(pInt, vInt);
  enc.encode(pLong, vLong);

  verifyDecode(pInt, vComplex, enc);
  verifyDecode(pLong, vComplex, enc);
}

TEST(EncodeTest, encodeSingleVal)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  double valDouble = -5.4;
  int valInt = -5;

  vector<complex<double>> expected1(he.slotCount(), valDouble);
  vector<complex<double>> expected2(he.slotCount(), valInt);

  PTile pDouble(he), pInt(he);

  enc.encode(pDouble, valDouble);
  enc.encode(pInt, valInt);

  verifyDecode(pDouble, expected1, enc);
  verifyDecode(pInt, expected2, enc);
}

TEST(EncoderTest, encryptAndDecryptDouble)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v{2, 2, 2};

  PTile p1(he);
  enc.encode(p1, v);

  CTile c(he);
  enc.encrypt(c, p1);

  PTile p2(he);
  enc.decrypt(p2, c);

  const std::vector<double> vals = enc.decodeDouble(p2);
  EXPECT_FLOAT_EQ(2, vals[0]);
}

TEST(EncoderTest, encryptAndDecryptComplex)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsComplexNumbers()) {
    return;
  }

  Encoder enc(he);

  using namespace std::complex_literals;
  std::vector<complex<double>> v{2. + 2i, 2. + 2i, 2. + 2i};

  PTile p1(he);
  enc.encode(p1, v);

  CTile c(he);
  enc.encrypt(c, p1);

  PTile p2(he);
  enc.decrypt(p2, c);

  const std::vector<complex<double>> vals = enc.decodeComplex(p2);
  EXPECT_FLOAT_EQ(2, vals[0].real());
  EXPECT_FLOAT_EQ(2, vals[0].imag());
}

TEST(EncoderTest, encodeEncryptAndDecryptDecodeDouble)
{
  std::vector<double> v{2.51, 3.2, -5.3};
  std::vector<long> vLong{3, 3, -5};
  std::vector<int> vInt{3, 3, -5};

  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  CTile c(he);
  enc.encodeEncrypt(c, v);

  const std::vector<double> valsDouble = enc.decryptDecodeDouble(c);
  const std::vector<int> valsInt = enc.decryptDecodeInt(c);
  const std::vector<long> valsLong = enc.decryptDecodeLong(c);

  for (size_t i = 0; i < v.size(); i++) {
    EXPECT_FLOAT_EQ(v[i], valsDouble[i]);
    EXPECT_EQ(vLong[i], valsLong[i]);
    EXPECT_EQ(vInt[i], valsInt[i]);
  }
}

TEST(EncoderTest, encodeEncryptAndDecryptDecodeComplex)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsComplexNumbers()) {
    return;
  }

  Encoder enc(he);

  using namespace std::complex_literals;
  std::vector<complex<double>> v{2. + 2i, 2. + 2i, 2. + 2i};

  CTile c(he);
  enc.encodeEncrypt(c, v);

  const std::vector<complex<double>> vals = enc.decryptDecodeComplex(c);
  EXPECT_FLOAT_EQ(2, vals[0].real());
  EXPECT_FLOAT_EQ(2, vals[0].imag());
}

TEST(EncoderTest, assertEquals)
{
  HeContext& he = TestUtils::getHighNumSlots();

  std::vector<double> v1{1, 1, 1};
  std::vector<double> v2{2, 2, 2};

  CTile c1(he);
  Encoder enc(he);
  enc.encodeEncrypt(c1, v1);

  double maxDiff = enc.assertEquals(c1, "test1", v1);
  EXPECT_LT(maxDiff, TestUtils::getEps());

  cout << "Negative test below, expecting an error to be printed" << endl;
  EXPECT_ANY_THROW(enc.assertEquals(c1, "test2", v2));
}

TEST(EncoderTest, decryptAddedNoise)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  if (he.getTraits().getSupportsDecryptAddedNoise()) {
    EXPECT_EQ(enc.getDecryptAddedNoiseEnabled(), true);
    EXPECT_EQ(enc.getDecryptAddedNoisePrecision(), -1);

    enc.setDecryptAddedNoisePrecision(30);
    EXPECT_EQ(enc.getDecryptAddedNoisePrecision(), 30);

    enc.setDecryptAddedNoiseEnabled(false);
    EXPECT_EQ(enc.getDecryptAddedNoiseEnabled(), false);
  } else {
    EXPECT_EQ(enc.getDecryptAddedNoiseEnabled(), false);
  }
}
}
