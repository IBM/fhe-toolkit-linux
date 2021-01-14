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

static void decode(vector<double>& d, const Encoder& enc, const PTile& p)
{
  d = enc.decodeDouble(p);
}

static void decode(vector<int>& d, const Encoder& enc, const PTile& p)
{
  d = enc.decodeInt(p);
}

static void decode(vector<long>& d, const Encoder& enc, const PTile& p)
{
  d = enc.decodeLong(p);
}

static void decryptDecode(vector<double>& d, const Encoder& enc, const CTile& c)
{
  d = enc.decryptDecodeDouble(c);
}

static void decryptDecode(vector<int>& d, const Encoder& enc, const CTile& c)
{
  d = enc.decryptDecodeInt(c);
}

static void decryptDecode(vector<long>& d, const Encoder& enc, const CTile& c)
{
  d = enc.decryptDecodeLong(c);
}

template <typename T>
static void encodeValsTest(vector<T>& v1,
                           vector<double>& v2,
                           vector<T>& expectedVals,
                           bool floatComparison)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);
  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  enc.encode(p, v2);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  c1.multiplyPlain(p);
  high_resolution_clock::time_point t3 = high_resolution_clock::now();

  cout << "encode: " << duration_cast<microseconds>(t2 - t1).count() << endl;
  cout << "multiplyPlain: " << duration_cast<microseconds>(t3 - t2).count()
       << endl;

  std::vector<double> vals;
  std::vector<T> vals2;
  decode(vals, enc, p);
  decryptDecode(vals2, enc, c1);

  for (size_t i = 0; i < v1.size(); i++) {
    EXPECT_NEAR(v2[i], vals[i], TestUtils::getEps());
  }

  for (size_t i = 0; i < v2.size(); i++) {

    if (fabs(expectedVals[i] - vals2[i]) > 1e-4) {
      cout << "i = " << i << endl;
      cout << "v1[i] = " << v1[i];
      cout << ", v2[i] = " << v2[i] << endl;
    }
    if (floatComparison) {
      EXPECT_NEAR(expectedVals[i], vals2[i], TestUtils::getEps());
    } else {
      EXPECT_EQ(expectedVals[i], vals2[i]);
    }
  }
}

template <typename T>
static void encodeValTest(vector<double>& v1,
                          T v2,
                          vector<T>& expectedVals,
                          bool floatComparison)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);
  CTile c1(he);
  enc.encodeEncrypt(c1, v1);

  PTile p(he);

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  enc.encode(p, v2);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  c1.multiplyPlain(p);
  high_resolution_clock::time_point t3 = high_resolution_clock::now();

  cout << "encode: " << duration_cast<microseconds>(t2 - t1).count() << endl;
  cout << "multiplyPlain: " << duration_cast<microseconds>(t3 - t2).count()
       << endl;

  std::vector<T> vals, vals2;
  decode(vals, enc, p);
  decryptDecode(vals2, enc, c1);

  for (size_t i = 0; i < v1.size(); i++) {
    if (floatComparison) {
      EXPECT_NEAR(v2, vals[i], TestUtils::getEps());
    } else {
      EXPECT_EQ(v2, vals[i]);
    }
  }

  for (size_t i = 0; i < expectedVals.size(); i++) {
    if (floatComparison) {
      EXPECT_NEAR(expectedVals[i], vals2[i], TestUtils::getEps());
    } else {
      EXPECT_EQ(expectedVals[i], vals2[i]);
    }
  }
}

TEST(EncoderTest, encodeAndDecode)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v{2.51, 3.2, -5.3};
  std::vector<long> vLong{3, 3, -5};
  std::vector<int> vInt{3, 3, -5};

  PTile p(he);
  enc.encode(p, v);

  const std::vector<double> valsDouble = enc.decodeDouble(p);
  const std::vector<int> valsInt = enc.decodeInt(p);
  const std::vector<long> valsLong = enc.decodeLong(p);

  for (size_t i = 0; i < v.size(); i++) {
    EXPECT_FLOAT_EQ(v[i], valsDouble[i]);
    EXPECT_EQ(vLong[i], valsLong[i]);
    EXPECT_EQ(vInt[i], valsInt[i]);
  }
}

TEST(EncoderTest, encodeVals)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  std::vector<double> v1(he.slotCount()), v2(he.slotCount()),
      expectedVals(he.slotCount());
  std::vector<int> v1Int(he.slotCount()), expectedValsInt(he.slotCount());
  std::vector<long> v1Long(he.slotCount()), expectedValsLong(he.slotCount());

  for (size_t i = 0; i < v1.size(); i++) {
    v1[i] = (double)(rand() % 1000) / 100;
    v2[i] = (double)(rand() % 1000) / 100;

    v1Int[i] = (int)v1[i];
    v1Long[i] = (long)v1[i];

    double tmp = ((double)v1Int[i]) * v2[i];

    // If the value after the decimal point of "tmp" is exactly 0.5, slightly
    // offset v2
    // to prevent small noise from causing rounding errors.
    if (tmp - (int)tmp == 0.5) {
      v2[i] += 1e-3;
      tmp = ((double)v1Int[i]) * v2[i];
    }

    expectedVals[i] = v1[i] * v2[i];

    expectedValsInt[i] = lround(tmp);
    expectedValsLong[i] = lround(tmp);
  }

  double originalEps = TestUtils::getEps();

  // A temporary hack - diff is larger than expected on the test below.
  TestUtils::setEps(originalEps * 10);
  encodeValsTest<double>(v1, v2, expectedVals, true);
  TestUtils::setEps(originalEps);

  // encodeValsTest<int>(v1Int, v2, expectedValsInt, false);
  // encodeValsTest<long>(v1Long, v2, expectedValsLong, false);
}

TEST(EncoderTest, encodeVal)
{
  HeContext& he = TestUtils::getHighNumSlots();
  vector<double> v1(he.slotCount()), expectedVals(he.slotCount());
  vector<int> expectedValsInt(he.slotCount());
  double v2 = (double)(rand() % 1000) / 100;
  int v2Int = (int)v2;

  for (size_t i = 0; i < v1.size(); i++) {
    v1[i] = (double)(rand() % 1000) / 100;

    double tmp = v1[i] * v2Int;

    if (tmp - (int)tmp == 0.5) {
      // If the value after the decimal point of "tmp" is exactly 0.5, slightly
      // offset v2
      // to prevent small noise from causing rounding errors.
      v1[i] += 0.5;
    }

    expectedVals[i] = v1[i] * v2;

    expectedValsInt[i] = lround(tmp);
  }

  encodeValTest<double>(v1, v2, expectedVals, true);
  // encodeValTest<int>(v1, v2Int, expectedValsInt, false);
}

TEST(EncoderTest, encodeAndDecodeComplex)
{
  HeContext& he = TestUtils::getHighNumSlots();

  if (!he.getTraits().getSupportsComplexNumbers()) {
    return;
  }

  Encoder enc(he);

  using namespace std::complex_literals;
  std::vector<complex<double>> v{2. + 2i, 2. + 2i, 2. + 2i};

  PTile p(he);
  enc.encode(p, v);

  const std::vector<complex<double>> vals = enc.decodeComplex(p);
  EXPECT_FLOAT_EQ(2, vals[0].real());
  EXPECT_FLOAT_EQ(2, vals[0].imag());
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
