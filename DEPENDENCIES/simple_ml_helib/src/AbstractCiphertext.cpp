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

#include <algorithm>

using namespace std;

void AbstractCiphertext::innerSum(int n) {
  innerSum(1,n);
}

void AbstractCiphertext::innerSum(int rot1, int rot2, bool reverse) {
  for(int rot=rot1; rot<rot2; rot*=2){
  	shared_ptr<AbstractCiphertext> tmp = clone();
    tmp->rotate(reverse?(-rot):rot);
    add(*tmp);
  }
}

void AbstractCiphertext::sumExpBySquaringLeftToRight(int n) {
  int rotateCount = 0;
  if (n == 0) {
    cout << "sumExpBySquaringLeftToRight() finished with " << rotateCount << " rotations" << endl;
    return;
  }

  vector<int> bits;
  while (n > 0) {
    bits.push_back(n%2);
    n/=2;
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
    if(bits[j]) {
      // 5  w <- v + (w >>> 1), e <- e + 1
      rotateCount++;
      rotate(1);
      add(*v);
      e++;
    }
  }

  // cout << "sumExpBySquaringLeftToRight() finished with " << rotateCount << " rotations" << endl;
}

void AbstractCiphertext::sumExpBySquaringRightToLeft(int n) {
  int rotateCount = 0;
  if (n == 0) {
    // cout << "sumExpBySquaringRightToLeft() finished with " << rotateCount << " rotations"
    // << endl;
    return;
  }

  bool firstY = true;
  shared_ptr<AbstractCiphertext> y;
  int currExp = 1;

  while (n > 1) {
    if (n%2){
      if (firstY) {
        y = clone();
        firstY = false;
      }
      else {
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
    // cout << "sumExpBySquaringRightToLeft() finished with " << rotateCount << " rotations" << endl;
    // If "firstY" is true, all the above updates updated "this" AbstractCipher directly, and
    // y was not used.
    // Note that "firstY" is expected to be true in the case in where n is a power of two.
    return;
  }
  else {
    rotateCount++;
    y->rotate(currExp);
    add(*y);
  }
  // cout << "sumExpBySquaringRightToLeft() finished with " << rotateCount << " rotations" << endl;
}

void AbstractCiphertext::debugPrint(const string& title, int maxVals, ostream& out) const {
  vector<complex<double>> res = decryptComplex();
  out << title << ": " << " slots=" << res.size() << ", chainIndex=" << getChainIndex() << endl;
  if (maxVals < 0)
    maxVals = res.size();
  for (int i = 0; i < min(maxVals, (int) res.size()); ++i)
    out << " " << res[i];
  out << endl;
}

double AbstractCiphertext::assertEquals(const std::string& title, const std::vector<double>& expectedVals, double eps, bool percent) const {
//  cout << "Abstract Cipher Assert Equals on " << title << endl;

  std::vector<double> vals = decrypt();
  if(expectedVals.size() > vals.size())
  	throw runtime_error("Size of expected values vector is bigger than size of cipher");

  double maxDiff = 0;
  for(size_t i=0; i<expectedVals.size(); ++i){
  	double diff = abs(vals[i] - expectedVals[i]);
  	double relDiff;
  	if (vals[i]!=0)
  		relDiff = diff / abs(vals[i]);
  	else
  		relDiff = abs(expectedVals[i]);
  	if ((percent && (relDiff > eps)) || (!percent && (diff > eps))) {
  	  if (percent)
  		  cout << "Error in checking relative diff:" << endl;
  	  else
  		  cout << "Error in checking diff:" << endl;

      cout << title << ", at slot " << i << ", expected value: " << expectedVals[i] <<
      		", actual value: " << vals[i] <<
			", diff: " << diff <<
			", relative-diff: " << relDiff <<
			", epsilon: " << eps << endl;
      throw runtime_error("Assert Equals Failed");
  	}
  	maxDiff = max(maxDiff, diff);
  }

//  cout << "TEST OK: " << title << " maxDiff=" << maxDiff << " ( " << expectedVals.size() << " elements tested)" << endl;
  return maxDiff;
}

void AbstractCiphertext::subAndAdjustChainIndex(AbstractCiphertext& other) {
     int thisIndex = getChainIndex(); 
     int thatIndex = other.getChainIndex(); 
   
     if (thisIndex > thatIndex) {
       setChainIndex(thatIndex);
       sub(other);   
     }
     else if (thatIndex > thisIndex) {
       shared_ptr<AbstractCiphertext> newother = other.clone();
       newother->setChainIndex(thisIndex);
       sub(*newother); 
     }
     else
       sub(other);   
}
void AbstractCiphertext::addAndAdjustChainIndex(AbstractCiphertext& other) {
     int thisIndex = getChainIndex(); 
     int thatIndex = other.getChainIndex(); 
   
     if (thisIndex > thatIndex) {
       setChainIndex(thatIndex);
       add(other);   
     }
     else if (thatIndex > thisIndex) {
       shared_ptr<AbstractCiphertext> newother = other.clone();
       newother->setChainIndex(thisIndex);
       add(*newother);   
     }
     else
       add(other);   
}
void AbstractCiphertext::multiplyAndAdjustChainIndex(AbstractCiphertext& other) {
     int thisIndex = getChainIndex(); 
     int thatIndex = other.getChainIndex(); 
   
     if (thisIndex > thatIndex) {
       setChainIndex(thatIndex);
       multiply(other);   
     }
     else if (thatIndex > thisIndex) {
       shared_ptr<AbstractCiphertext> newother = other.clone();
       newother->setChainIndex(thisIndex);
       multiply(*newother);   
     }
     else
       multiply(other);   
}

void AbstractCiphertext::multiplyPlainAndAdjustChainIndex(AbstractPlaintext& plain) {
     int thisIndex = getChainIndex(); 
     int thatIndex = plain.getChainIndex(); 
   
     if (thisIndex > thatIndex) {
       setChainIndex(thatIndex);
       multiplyPlain(plain);   
     }
     else if (thatIndex > thisIndex) {
       shared_ptr<AbstractPlaintext> newplain = plain.clone();
       newplain->setChainIndex(thisIndex);
       multiplyPlain(*newplain);   
     }
     else
       multiplyPlain(plain);   
}
