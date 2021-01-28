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

//
// INFO: The API used to create these examples can be found online 
// ML-HElib: https://ibm.github.io/fhe-toolkit-linux”
// HElib: https://ibm.github.io/fhe-toolkit-linux/html/helib/index.html
//

#include "helayers/hebase/helib/HelibCkksContext.h"
#include "helayers/hebase/hebase.h"

using namespace std;
using namespace helayers;

void tut_2_run(HeContext& he);

void tut_2_plaintexts()
{

  // As in tut_1_basics, let's choose a default HE CKKS setup
  shared_ptr<HeContext> hePtr = HelibContext::create(HELIB_CKKS_8192);

  // Let's also make sure we have enough security
  always_assert(hePtr->getSecurityLevel() >= 128);

  // and do some work
  tut_2_run(*hePtr);
}

void tut_2_run(HeContext& he)
{

  // We'll start by encrypting some values
  vector<double> vals1{1.0, 2.0, 3.0};
  Encoder encoder(he);
  CTile c1(he);
  encoder.encodeEncrypt(c1, vals1);
  // c1 now contains the vector (1,2,3,0,0,0,....) encrypted.

  // we now want to multiply each slot by 4.
  // one way to do it is to encrypt the vector (4,4,4,0,0,0....) to c2
  // and exeucte c1.multiply(c2),
  // but ciphertext-ciphertext multiplication is expensive
  // There are three things that might make our operation cheaper:
  //   - we want to multiply by a known (non-encrypted) value of 4.
  //   - 4 is an integer
  //   - 4 is a scalar, i.e., all slots are multiplied by the same value.
  // So here's how it's done:
  CTile c2(c1); // we copy c1 to c2 to keep the original for more examples
  c2.multiplyScalar(4);
  // this operation is both time efficient and consumes minimal resources
  // inside the ciphertext.
  // Let's verify it worked:
  encoder.assertEquals(c2, "multScalar", vector<double>{4, 8, 12});
  c2.debugPrint("after multiplyScalar");

  c2 = c1;
  // multiplying by a double number is a bit more expensive
  c2.multiplyScalar(2.5);
  encoder.assertEquals(c2, "multScalar double", vector<double>{2.5, 5, 7.5});
  c2.debugPrint("after multiplyScalar double");

  // say we want to multiply our c1 encrypted vector with a non-encrypted vector
  // (4,5,6,...)
  vector<double> clearVals{4, 5, 6};
  // to multiply each slot by a different plaintext number, a slightly more
  // expensive
  // operation is required, but still cheaper than ciphertext-ciphertext
  // multiplication
  // First we create PTile, an encoded, but not-encrypted form of our vector
  PTile p(he);
  encoder.encode(p, clearVals);

  // now we multiply c2 by p using multiplyPlain:
  c2 = c1;
  c2.multiplyPlain(p);
  encoder.assertEquals(c2, "multiplyPlain", vector<double>{4, 10, 18});
  c2.debugPrint("after multiplyPlain");

  // add and sub for plaintexts also supported:
  c2 = c1;
  c2.addPlain(p);
  encoder.assertEquals(c2, "addPlain", vector<double>{5, 7, 9});
  c2.debugPrint("after add plain");

  c2 = c1;
  c2.subPlain(p);
  encoder.assertEquals(c2, "subPlain", vector<double>{-3, -3, -3});
  c2.debugPrint("after subPlain");

  // The encode operation that created p is itself non-trivially time consuming.
  // You might consider caching PTile objects for re-use, if needed.
}