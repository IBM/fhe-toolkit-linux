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

#include <iostream>
#include <fstream>
#include "helayers/hebase/hebase.h"
#include "TestUtils.h"
#include "gtest/gtest.h"

using namespace helayers;
using namespace std;

namespace helayerstest {

static void saveLoadTest(bool withSecKey)
{
  HeContext& he = TestUtils::getLowNumSlots();
  std::vector<double> v(he.slotCount()), v2(he.slotCount());

  for (double& val : v) {
    val = ((double)(rand() % 1000)) / 1000;
  }

  for (double& val : v2) {
    val = ((double)(rand() % 1000)) / 1000;
  }

  CTile c(he);
  Encoder enc(he);
  enc.encodeEncrypt(c, v);

  TestUtils::createOutputDirectory();
  std::string contextFile = TestUtils::getOutputDirectory() + "/HeContext.tmp";
  std::string secKeyFile = TestUtils::getOutputDirectory() + "/secKey.tmp";

  he.saveToFile(contextFile, withSecKey);
  if (!withSecKey) {
    // Save the secret key to a separate file.
    // Note that if "withSecKey" is fales, when loading "he2" below, it will
    // be loaded without a secret key. In this case, we store and load the
    // secret key to a separate file to enable decryption.
    he.saveSecretKeyToFile(secKeyFile);
  }

  shared_ptr<HeContext> he2 = TestUtils::heContextFactory->create();
  // Load the original heContext into a new one
  he2->loadFromFile(contextFile);

  if (withSecKey) {
    EXPECT_EQ(he2->hasSecretKey(), true);
  } else {
    EXPECT_EQ(he2->hasSecretKey(), false);
  }

  // Save "c" in order to load it with a new heContext
  std::string cTileFile = TestUtils::getOutputDirectory() + "/cTile.tmp";

  c.saveToFile(cTileFile);

  // Try loading the original ciphertext (which had the original context) into
  // a new cipherText with the new context.
  // Decryption result should be the same.
  CTile cCopy(*he2);
  cCopy.loadFromFile(cTileFile);

  Encoder enc2(*he2);

  // Try multiplying and rotating, to check that other keys work as expected.
  /// Note that the operations below should work even without a secret key
  CTile c2(*he2), expectedC2(he);
  enc.encodeEncrypt(expectedC2, v2);
  enc2.encodeEncrypt(c2, v2);

  expectedC2.multiply(c);
  c2.multiply(cCopy);

  if (!withSecKey) {
    // Load secret key to enable decryption
    he2->loadSecretKeyFromFile(secKeyFile);
  }

  EXPECT_EQ(he2->hasSecretKey(), true);

  Encoder enc2WithSecKey(*he2);

  std::vector<double> expectedVals = enc.decryptDecodeDouble(c);
  std::vector<double> vals = enc2WithSecKey.decryptDecodeDouble(cCopy);

  EXPECT_EQ(expectedVals.size(), vals.size());

  for (size_t i = 0; i < v.size(); i++) {
    EXPECT_NEAR(expectedVals[i], vals[i], TestUtils::getEps());
  }

  expectedVals = enc.decryptDecodeDouble(expectedC2);
  vals = enc2WithSecKey.decryptDecodeDouble(c2);

  EXPECT_EQ(expectedVals.size(), vals.size());

  for (size_t i = 0; i < v.size(); i++) {
    EXPECT_NEAR(expectedVals[i], vals[i], TestUtils::getEps());
  }

  c2.rotate(10);
  expectedC2.rotate(10);

  expectedVals = enc.decryptDecodeDouble(expectedC2);
  vals = enc2WithSecKey.decryptDecodeDouble(c2);

  EXPECT_EQ(v.size(), vals.size());

  for (size_t i = 0; i < v.size(); i++) {
    EXPECT_NEAR(expectedVals[i], vals[i], TestUtils::getEps());
  }
}

TEST(HeContextTest, saveLoadNoSecKey)
{
  cout << "Note: when running with some contexts this test might produce "
          "warnings. That's ok. "
       << endl;
  saveLoadTest(false);
}

TEST(HeContextTest, saveLoadWithSecKey) { saveLoadTest(true); }
} // namespace helayerstest
