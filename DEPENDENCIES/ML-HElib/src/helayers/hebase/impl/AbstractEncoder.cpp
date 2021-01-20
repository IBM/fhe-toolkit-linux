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

#include "AbstractEncoder.h"

using namespace std;

namespace helayers {

AbstractEncoder::AbstractEncoder() {}

AbstractEncoder::~AbstractEncoder() {}

template <typename T>
static vector<T> roundAndCastVector(const vector<double>& vec)
{
  vector<T> castedVec;
  castedVec.reserve(vec.size());

  for (const double& val : vec)
    castedVec.push_back(lround(val));

  return castedVec;
}

template <typename SrcType, typename DistType>
static vector<DistType> castVector(const vector<SrcType>& vec)
{
  vector<DistType> castedVec;
  castedVec.reserve(vec.size());

  for (const SrcType& val : vec)
    castedVec.push_back(static_cast<DistType>(val));

  return castedVec;
}

void AbstractEncoder::setDefaultScale(double scale)
{
  throw runtime_error("setDefaultScale is not supported by this encoder");
}

double AbstractEncoder::getDefaultScale() const
{
  throw runtime_error("getDefaultScale is not supported by this encoder");
}

void AbstractEncoder::restoreDefaultScale()
{
  throw runtime_error("restoreDefaultScale is not supported by this encoder");
}

void AbstractEncoder::setDecryptAddedNoiseEnabled(bool val)
{
  if (val)
    throw runtime_error("Enabling decrypt added noise not supported");
}

bool AbstractEncoder::getDecryptAddedNoiseEnabled() const { return false; }

void AbstractEncoder::setDecryptAddedNoisePrecision(int val)
{
  throw runtime_error("Decrypt added noise precision not supported");
}

int AbstractEncoder::getDecryptAddedNoisePrecision() const
{
  throw runtime_error("Decrypt added noise precision not supported");
}

// below are default implementations for encoding a single value,
// which simply duplicates it into a vector and call the regular vector
// encoding.
// specific optimized implementations can be defined on derived classes.

void AbstractEncoder::encode(AbstractPlaintext& res,
                             int val,
                             int chainIndex) const
{
  std::vector<double> vals(res.slotCount(), val);
  encode(res, vals, chainIndex);
}

void AbstractEncoder::encode(AbstractPlaintext& res,
                             double val,
                             int chainIndex) const
{
  std::vector<double> vals(res.slotCount(), val);
  encode(res, vals, chainIndex);
}

void AbstractEncoder::encode(AbstractPlaintext& res,
                             const std::vector<int>& vals,
                             int chainIndex) const
{
  encode(res, castVector<int, double>(vals), chainIndex);
}

void AbstractEncoder::encode(AbstractPlaintext& res,
                             const std::vector<long>& vals,
                             int chainIndex) const
{
  encode(res, castVector<long, double>(vals), chainIndex);
}

vector<int> AbstractEncoder::decodeInt(const AbstractPlaintext& src) const
{
  return roundAndCastVector<int>(decodeDouble(src));
}
vector<long> AbstractEncoder::decodeLong(const AbstractPlaintext& src) const
{
  return roundAndCastVector<long>(decodeDouble(src));
}

void AbstractEncoder::encodeEncrypt(AbstractCiphertext& res,
                                    const std::vector<int>& vals,
                                    int chainIndex) const
{
  encodeEncrypt(res, castVector<int, double>(vals), chainIndex);
}

void AbstractEncoder::encodeEncrypt(AbstractCiphertext& res,
                                    const std::vector<long>& vals,
                                    int chainIndex) const
{
  encodeEncrypt(res, castVector<long, double>(vals), chainIndex);
}

std::vector<int> AbstractEncoder::decryptDecodeInt(
    const AbstractCiphertext& src) const
{
  return roundAndCastVector<int>(decryptDecodeDouble(src));
}

std::vector<long> AbstractEncoder::decryptDecodeLong(
    const AbstractCiphertext& src) const
{
  return roundAndCastVector<long>(decryptDecodeDouble(src));
}

double AbstractEncoder::assertEquals(const AbstractCiphertext& c,
                                     const string& title,
                                     const std::vector<long>& expectedVals,
                                     double eps,
                                     bool percent) const
{
  std::vector<complex<double>> vals = decryptDecodeComplex(c);
  return assertEqualsHelper(title,
                            vals,
                            castVector<long, complex<double>>(expectedVals),
                            eps,
                            percent);
}

double AbstractEncoder::assertEquals(const AbstractCiphertext& c,
                                     const string& title,
                                     const std::vector<int>& expectedVals,
                                     double eps,
                                     bool percent) const
{
  std::vector<complex<double>> vals = decryptDecodeComplex(c);
  return assertEqualsHelper(title,
                            vals,
                            castVector<int, complex<double>>(expectedVals),
                            eps,
                            percent);
}

double AbstractEncoder::assertEquals(const AbstractCiphertext& c,
                                     const string& title,
                                     const std::vector<double>& expectedVals,
                                     double eps,
                                     bool percent) const
{
  std::vector<complex<double>> vals = decryptDecodeComplex(c);
  std::vector<complex<double>> newExpectedVals;
  for (const auto& val : expectedVals)
    newExpectedVals.push_back(val);
  return assertEqualsHelper(title, vals, newExpectedVals, eps, percent);
}

double AbstractEncoder::assertEquals(
    const AbstractCiphertext& c,
    const string& title,
    const std::vector<complex<double>>& expectedVals,
    double eps,
    bool percent) const
{
  std::vector<complex<double>> vals = decryptDecodeComplex(c);
  return assertEqualsHelper(title, vals, expectedVals, eps, percent);
}

double AbstractEncoder::assertEqualsHelper(
    const string& title,
    const std::vector<complex<double>>& vals,
    const std::vector<complex<double>>& expectedVals,
    double eps,
    bool percent) const
{
  if (expectedVals.size() > vals.size())
    throw runtime_error(
        "Size of expected values vector is bigger than size of cipher");

  double maxDiff = 0;
  for (size_t i = 0; i < expectedVals.size(); ++i) {
    double diff = abs(vals[i] - expectedVals[i]);
    double relDiff;
    if (abs(vals[i]) != 0)
      relDiff = diff / abs(vals[i]);
    else
      relDiff = abs(expectedVals[i]);
    if ((percent && (relDiff > eps)) || (!percent && (diff > eps))) {
      if (percent)
        cout << "Error in checking relative diff:" << endl;
      else
        cout << "Error in checking diff:" << endl;

      cout << title << ", at slot " << i
           << ", expected value: " << expectedVals[i]
           << ", actual value: " << vals[i] << ", diff: " << diff
           << ", relative-diff: " << relDiff << ", epsilon: " << eps << endl;
      throw runtime_error("Assert Equals Failed");
    }
    maxDiff = max(maxDiff, diff);
  }

  //  cout << "TEST OK: " << title << " maxDiff=" << maxDiff << " ( " <<
  //  expectedVals.size() << " elements tested)" << endl;
  return maxDiff;
}
}
