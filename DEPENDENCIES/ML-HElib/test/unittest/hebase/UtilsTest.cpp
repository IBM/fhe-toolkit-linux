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

#include "TestUtils.h"
#include "gtest/gtest.h"
//#include "helayers/hebase/hebase.h"
#include "helayers/hebase/utils/JsonWrapper.h"

using namespace helayers;
using namespace std;

namespace helayerstest {

const string jsonExample = R"json({
    "a1":"x1",
    "a2":"x2",
    "a3":123,
    "a4":12.3,
    "a5":{
      "b1":"y1"
    }
    }
  )json";

TEST(UtilsTest, jsonGetters)
{
  JsonWrapper jw;
  jw.load(jsonExample);
  EXPECT_EQ("x1", jw.getString("a1"));
  EXPECT_EQ("x2", jw.getString("a2"));
  EXPECT_EQ(123, jw.getInt("a3"));
  EXPECT_FLOAT_EQ(12.3, jw.getDouble("a4"));
  EXPECT_EQ("y1", jw.getString("a5.b1"));
}

TEST(UtilsTest, jsonGettersErrors)
{
  JsonWrapper jw;
  jw.load(jsonExample);
  EXPECT_ANY_THROW(jw.getInt("a1"));
  EXPECT_ANY_THROW(jw.getDouble("a1"));
  EXPECT_ANY_THROW(jw.getInt("a4"));
  EXPECT_ANY_THROW(jw.getString("foo"));
}

TEST(UtilsTest, jsonLoadChar)
{
  const char* data = R"json({
    "a1":"x1"
    }
  )json";
  JsonWrapper jw;
  int len = strlen(data);
  jw.load(data, len);
  EXPECT_EQ("x1", jw.getString("a1"));
}
TEST(UtilsTest, jsonBinIo)
{

  JsonWrapper jw;
  jw.load(jsonExample);
  stringstream buf;
  jw.writeToBinary(buf);
  string storedData = buf.str();
  stringstream readBuf(storedData);
  JsonWrapper jw2;
  jw2.loadFromBinary(readBuf);
  EXPECT_EQ("x1", jw2.getString("a1"));
  EXPECT_FLOAT_EQ(12.3, jw.getDouble("a4"));
}
} // namespace helayerstest
