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

#include "Encoder.h"

using namespace std;

namespace helayers {

Encoder::Encoder(HeContext& he) : he(he), impl(he.getEncoder()) {}

Encoder::~Encoder() {}

int Encoder::validateChainIndex(int chainIndex) const
{
  if (he.getTraits().getAutomaticallyManagesChainIndices())
    return 0; // not relevant and will not be used
  else if (chainIndex < -1 || chainIndex > he.getTopChainIndex())
    throw invalid_argument("Illegal chain index value " +
                           to_string(chainIndex) +
                           " where max=" + to_string(he.getTopChainIndex()));
  else if (chainIndex == -1)
    return he.getTopChainIndex();
  else
    return chainIndex;
}

template <typename T>
bool Encoder::checkEncodeVectorSize(const vector<T>& vec) const
{
  if (vec.size() > he.slotCount())
    throw runtime_error(
        "Can not encode a vector with more than he.slotCount() slots");
  if ((vec.size() < he.slotCount()) && !he.getTraits().getIsDebugEmpty())
    return false;
  return true;
}

template <typename T>
vector<T> Encoder::adjustEncodeVectorSize(const vector<T>& vec) const
{
  vector<T> vecCopy(vec);
  vecCopy.resize(he.slotCount(), 0);
  return vecCopy;
}

void Encoder::setDefaultScale(double scale) { impl->setDefaultScale(scale); }

double Encoder::getDefaultScale() const { return impl->getDefaultScale(); }

void Encoder::restoreDefaultScale() { return impl->restoreDefaultScale(); }

void Encoder::setDecryptAddedNoiseEnabled(bool val)
{
  impl->setDecryptAddedNoiseEnabled(val);
}

bool Encoder::getDecryptAddedNoiseEnabled() const
{
  return impl->getDecryptAddedNoiseEnabled();
}

void Encoder::setDecryptAddedNoisePrecision(int val)
{
  impl->setDecryptAddedNoisePrecision(val);
}

int Encoder::getDecryptAddedNoisePrecision() const
{
  return impl->getDecryptAddedNoisePrecision();
}

void Encoder::encode(PTile& res, int val, int chainIndex) const
{
  impl->encode(*res.impl, val, validateChainIndex(chainIndex));
}

void Encoder::encode(PTile& res, double val, int chainIndex) const
{
  impl->encode(*res.impl, val, validateChainIndex(chainIndex));
}

void Encoder::encode(PTile& res, const vector<int>& vals, int chainIndex) const
{
  impl->encode(*res.impl,
               checkEncodeVectorSize(vals) ? vals
                                           : adjustEncodeVectorSize(vals),
               validateChainIndex(chainIndex));
}

void Encoder::encode(PTile& res, const vector<long>& vals, int chainIndex) const
{
  impl->encode(*res.impl,
               checkEncodeVectorSize(vals) ? vals
                                           : adjustEncodeVectorSize(vals),
               validateChainIndex(chainIndex));
}

void Encoder::encode(PTile& res,
                     const vector<double>& vals,
                     int chainIndex) const
{
  impl->encode(*res.impl,
               checkEncodeVectorSize(vals) ? vals
                                           : adjustEncodeVectorSize(vals),
               validateChainIndex(chainIndex));
}

void Encoder::encode(PTile& res,
                     const vector<complex<double>>& vals,
                     int chainIndex) const
{
  impl->encode(*res.impl,
               checkEncodeVectorSize(vals) ? vals
                                           : adjustEncodeVectorSize(vals),
               validateChainIndex(chainIndex));
}

vector<int> Encoder::decodeInt(const PTile& src) const
{
  return impl->decodeInt(*src.impl);
}

vector<long> Encoder::decodeLong(const PTile& src) const
{
  return impl->decodeLong(*src.impl);
}

vector<double> Encoder::decodeDouble(const PTile& src) const
{
  return impl->decodeDouble(*src.impl);
}

vector<complex<double>> Encoder::decodeComplex(const PTile& src) const
{
  return impl->decodeComplex(*src.impl);
}

void Encoder::encrypt(CTile& res, const PTile& src) const
{
  impl->encrypt(*res.impl, *src.impl);
}

void Encoder::decrypt(PTile& res, const CTile& src) const
{
  impl->decrypt(*res.impl, *src.impl);
}

void Encoder::encodeEncrypt(CTile& res,
                            const vector<int>& vals,
                            int chainIndex) const
{
  impl->encodeEncrypt(
      *res.impl,
      checkEncodeVectorSize(vals) ? vals : adjustEncodeVectorSize(vals),
      validateChainIndex(chainIndex));
}

void Encoder::encodeEncrypt(CTile& res,
                            const vector<long>& vals,
                            int chainIndex) const
{
  impl->encodeEncrypt(
      *res.impl,
      checkEncodeVectorSize(vals) ? vals : adjustEncodeVectorSize(vals),
      validateChainIndex(chainIndex));
}

void Encoder::encodeEncrypt(CTile& res,
                            const vector<double>& vals,
                            int chainIndex) const
{
  impl->encodeEncrypt(
      *res.impl,
      checkEncodeVectorSize(vals) ? vals : adjustEncodeVectorSize(vals),
      validateChainIndex(chainIndex));
}

void Encoder::encodeEncrypt(CTile& res,
                            const vector<complex<double>>& vals,
                            int chainIndex) const
{
  impl->encodeEncrypt(
      *res.impl,
      checkEncodeVectorSize(vals) ? vals : adjustEncodeVectorSize(vals),
      validateChainIndex(chainIndex));
}

vector<int> Encoder::decryptDecodeInt(const CTile& src) const
{
  return impl->decryptDecodeInt(*src.impl);
}

vector<long> Encoder::decryptDecodeLong(const CTile& src) const
{
  return impl->decryptDecodeLong(*src.impl);
}

vector<double> Encoder::decryptDecodeDouble(const CTile& src) const
{
  return impl->decryptDecodeDouble(*src.impl);
}

vector<complex<double>> Encoder::decryptDecodeComplex(const CTile& src) const
{
  return impl->decryptDecodeComplex(*src.impl);
}

void Encoder::printErrorStats(CTile& actualC,
                              vector<double> expectedZ,
                              ostream& out,
                              bool asCsv) const
{
  vector<complex<double>> actualZ = decryptDecodeComplex(actualC);
  int SC = he.slotCount();
  double maxDiff = 0.0;
  double maxRelDiff = 0.0;
  double diff, relDiff;
  int maxDiffI = 0;
  int maxRelDiffI = 0;
  double maxDiffRel = 0.0;
  double sumDiff = 0.0;
  double sumRelDiff = 0.0;
  double sumDiffSqr = 0.0;
  double sumRelDiffSqr = 0.0;
  for (int i = 0; i < SC; i++) {
    diff = abs(actualZ[i].real() - expectedZ[i]);
    if (actualZ[i].real() != 0)
      relDiff = abs(100 * diff / actualZ[i].real());
    else
      relDiff = abs(100 * diff);
    sumDiff += diff;
    sumRelDiff += relDiff;
    sumDiffSqr += diff * diff;
    sumRelDiffSqr += relDiff * relDiff;
    if (diff > maxDiff) {
      maxDiff = diff;
      maxDiffI = i;
      maxDiffRel = relDiff;
    }
    if (relDiff > maxRelDiff) {
      maxRelDiff = relDiff;
      maxRelDiffI = i;
    }
  }

  if (asCsv) {
    out << maxDiff << ", " << sumDiff / SC << ", "
        << sqrt(abs((sumDiffSqr / SC) - ((sumDiff / SC) * (sumDiff / SC))));
  } else {
    out << "Max diff  : expected=" << expectedZ[maxDiffI]
        << " actual=" << actualZ[maxDiffI].real() << " diff=" << maxDiff
        << " diff%=" << maxDiffRel << "%" << endl;

    out << "Max diff% : expected=" << expectedZ[maxRelDiffI]
        << " actual=" << actualZ[maxRelDiffI].real()
        << " diff=" << abs(expectedZ[maxRelDiffI] - actualZ[maxRelDiffI].real())
        << " diff%=" << maxRelDiff << "%" << endl;

    out << "Mean diff :"
        << " diff=" << sumDiff / SC << " diff%= " << sumRelDiff / SC << endl;

    out << "STD diff  :"
        << " diff="
        << sqrt(abs((sumDiffSqr / SC) - ((sumDiff / SC) * (sumDiff / SC))))
        << " diff%= "
        << sqrt(abs((sumRelDiffSqr / SC) -
                    ((sumRelDiff / SC) * (sumRelDiff / SC))))
        << endl;
  }
}

double Encoder::assertEquals(const CTile& c,
                             const string& title,
                             const std::vector<int>& expectedVals,
                             double eps,
                             bool percent) const
{
  return impl->assertEquals(*c.impl, title, expectedVals, eps, percent);
}

double Encoder::assertEquals(const CTile& c,
                             const string& title,
                             const std::vector<long>& expectedVals,
                             double eps,
                             bool percent) const
{
  return impl->assertEquals(*c.impl, title, expectedVals, eps, percent);
}

double Encoder::assertEquals(const CTile& c,
                             const string& title,
                             const std::vector<double>& expectedVals,
                             double eps,
                             bool percent) const
{
  return impl->assertEquals(*c.impl, title, expectedVals, eps, percent);
}

double Encoder::assertEquals(const CTile& c,
                             const string& title,
                             const std::vector<complex<double>>& expectedVals,
                             double eps,
                             bool percent) const
{
  return impl->assertEquals(*c.impl, title, expectedVals, eps, percent);
}
} // namespace helayers
