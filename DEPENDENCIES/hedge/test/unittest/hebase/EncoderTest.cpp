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
#include "gtest/gtest.h"
#include "hedge/hebase/hebase.h"
#include "TestUtils.h"

using namespace std;
using namespace std::chrono;
using namespace hedge;

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

namespace hedgetest {

template <typename T>
static void encodeValsTest(vector<T>& v1,
                           vector<T>& v2,
                           T expectedVal1,
                           T expectedVal2,
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

  EXPECT_FLOAT_EQ(expectedVal1, vals[0]);

  EXPECT_FLOAT_EQ(expectedVal2, vals2[0]);
}

template <typename T>
static void encodeValTest(vector<T>& v1,
                          T v2,
                          T expectedVal1,
                          T expectedVal2,
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

  EXPECT_FLOAT_EQ(expectedVal1, vals[0]);

  EXPECT_FLOAT_EQ(expectedVal2, vals2[0]);
}

template <typename T>
static void encodeEncryptAndDecryptDecodeTest(vector<T>& v,
                                              bool floatComparison)
{
  HeContext& he = TestUtils::getHighNumSlots();
  Encoder enc(he);

  CTile c(he);
  enc.encodeEncrypt(c, v);

  std::vector<T> vals;
  decryptDecode(vals, enc, c);

  if (floatComparison) {
    EXPECT_FLOAT_EQ(v[0], vals[0]);
  } else {
    EXPECT_EQ(v[0], vals[0]);
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

  std::vector<double> v1(he.slotCount(), 2.3);
  std::vector<int> v1Int(he.slotCount(), 2);
  std::vector<long> v1Long(he.slotCount(), 2);

  std::vector<double> v2(he.slotCount(), 3.1);
  std::vector<int> v2Int(he.slotCount(), 3);
  std::vector<long> v2Long(he.slotCount(), 3);

  encodeValsTest<double>(v1, v2, 3.1, 2.3 * 3.1, true);
  encodeValsTest<int>(v1Int, v2Int, 3, 2 * 3, false);
  encodeValsTest<long>(v1Long, v2Long, 3, 2 * 3, false);
}

TEST(EncoderTest, encodeVal)
{
  HeContext& he = TestUtils::getHighNumSlots();
  vector<double> v1(he.slotCount(), 3.5);
  vector<int> v1Int(he.slotCount(), 3);
  double v2 = 2.7;
  int v2Int = 2;
  encodeValTest<double>(v1, v2, v2, v1[0] * v2, true);
  encodeValTest<int>(v1Int, v2Int, v2Int, v1Int[0] * v2Int, false);
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
  std::vector<double> v{2.5, 2.5, 2.5};
  std::vector<long> vLong{2, 2, 2};
  std::vector<int> vInt{2, 2, 2};

  encodeEncryptAndDecryptDecodeTest<double>(v, true);
  encodeEncryptAndDecryptDecodeTest<long>(vLong, false);
  encodeEncryptAndDecryptDecodeTest<int>(vInt, false);
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
}
