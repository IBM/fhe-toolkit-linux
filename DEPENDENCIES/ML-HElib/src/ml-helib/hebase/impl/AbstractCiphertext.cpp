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

#include "AbstractCiphertext.h"
#include "AbstractEncoder.h"
#include "hedge/hebase/HedgeTimer.h"

#include <algorithm>

using namespace std;

namespace hedge {

void AbstractCiphertext::innerSum(int n) { innerSum(1, n); }

void AbstractCiphertext::innerSum(int rot1, int rot2, bool reverse)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::innerSum");
  for (int rot = rot1; rot < rot2; rot *= 2) {
    shared_ptr<AbstractCiphertext> tmp = clone();
    tmp->rotate(reverse ? (-rot) : rot);
    add(*tmp);
  }
}

void AbstractCiphertext::multiplyByChangingScale(double factor)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::multiplyByChangingScale");
  setScale(getScale() / factor);
}

void AbstractCiphertext::sumExpBySquaringLeftToRight(int n)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::sumExpBySquaringLeftToRight");
  int rotateCount = 0;
  if (n == 0) {
    cout << "sumExpBySquaringLeftToRight() finished with " << rotateCount
         << " rotations" << endl;
    return;
  }

  vector<int> bits;
  while (n > 0) {
    bits.push_back(n % 2);
    n /= 2;
  }

  shared_ptr<AbstractCiphertext> v = clone();
  int e = 1;
  for (int j = bits.size() - 2; j >= 0; j--) {
    // 3  w <- w + (w >>> e), e <- 2*e
    shared_ptr<AbstractCiphertext> tmp = clone();
    rotateCount++;
    tmp->rotate(e);
    add(*tmp);
    e *= 2;
    if (bits[j]) {
      // 5  w <- v + (w >>> 1), e <- e + 1
      rotateCount++;
      rotate(1);
      add(*v);
      e++;
    }
  }

  // cout << "sumExpBySquaringLeftToRight() finished with " << rotateCount << "
  // rotations" << endl;
}

void AbstractCiphertext::sumExpBySquaringRightToLeft(int n)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::sumExpBySquaringRightToLeft");
  int rotateCount = 0;
  if (n == 0) {
    // cout << "sumExpBySquaringRightToLeft() finished with " << rotateCount <<
    // " rotations"
    // << endl;
    return;
  }

  bool firstY = true;
  shared_ptr<AbstractCiphertext> y;
  int currExp = 1;

  while (n > 1) {
    if (n % 2) {
      if (firstY) {
        y = clone();
        firstY = false;
      } else {
        shared_ptr<AbstractCiphertext> tmp = clone();
        rotateCount++;
        y->rotate(currExp);
        tmp->add(*y);
        y = tmp;
      }
    }

    shared_ptr<AbstractCiphertext> tmp = clone();
    rotateCount++;
    tmp->rotate(currExp);
    add(*tmp);
    currExp *= 2;
    n /= 2;
  }

  if (firstY) {
    // cout << "sumExpBySquaringRightToLeft() finished with " << rotateCount <<
    // " rotations" << endl;
    // If "firstY" is true, all the above updates updated "this" AbstractCipher
    // directly, and
    // y was not used.
    // Note that "firstY" is expected to be true in the case in where n is a
    // power of two.
    return;
  } else {
    rotateCount++;
    y->rotate(currExp);
    add(*y);
  }
  // cout << "sumExpBySquaringRightToLeft() finished with " << rotateCount << "
  // rotations" << endl;
}

void AbstractCiphertext::debugPrint(const string& title,
                                    int maxVals,
                                    int verbose,
                                    ostream& out) const
{
  HEDGE_TIMER("AbstractCiphertext::debugPrint");
  if (title.length() > 0)
    cout << title << endl;
  cout << "chain index         : " << getChainIndex() << endl;
  shared_ptr<AbstractEncoder> encoder = he.getEncoder();
  vector<complex<double>> res = encoder->decryptDecodeComplex(*this);
  cout << "slots               : " << res.size() << endl;

  if (maxVals < 0)
    maxVals = res.size();
  out << " ";
  for (int i = 0; i < min(maxVals, (int)res.size()); ++i)
    out << " " << res[i];
  if (maxVals < (res.size() - 1))
    out << " ... " << res[res.size() - 1];
  out << endl;
}

void AbstractCiphertext::addScalar(int scalar)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::addScalar(int)");
  shared_ptr<AbstractEncoder> encoder = he.getEncoder();
  shared_ptr<AbstractPlaintext> pt = he.createAbstractPlain();
  encoder->encode(*pt, scalar, getChainIndex());
  addPlain(*pt);
}

void AbstractCiphertext::addScalar(double scalar)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::addScalar(double)");
  shared_ptr<AbstractEncoder> encoder = he.getEncoder();
  shared_ptr<AbstractPlaintext> pt = he.createAbstractPlain();
  encoder->encode(*pt, scalar, getChainIndex());
  addPlain(*pt);
}

void AbstractCiphertext::multiplyScalar(int scalar)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::multiplyScalar(int)");
  shared_ptr<AbstractEncoder> encoder = he.getEncoder();
  shared_ptr<AbstractPlaintext> pt = he.createAbstractPlain();
  encoder->encode(*pt, scalar, getChainIndex());
  multiplyPlain(*pt);
}

void AbstractCiphertext::multiplyScalar(double scalar)
{
  HEDGE_TIMER_SECTION("AbstractCiphertext::multiplyScalar(double)");
  shared_ptr<AbstractEncoder> encoder = he.getEncoder();
  shared_ptr<AbstractPlaintext> pt = he.createAbstractPlain();
  encoder->encode(*pt, scalar, getChainIndex());
  multiplyPlain(*pt);
}
}
