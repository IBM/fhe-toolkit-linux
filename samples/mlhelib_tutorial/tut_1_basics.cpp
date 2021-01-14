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

#include "helayers/hebase/helib/HelibCkksContext.h"
#include "helayers/hebase/hebase.h"

// This tutorial shows a basic usage example of using the hebase layer.
// For our basic example, we'll configure HElib as the underlying HE library
// and CKKS as the scheme.

using namespace std;
using namespace helayers;

void tut_1_run(HeContext& he);

void tut_1_basics()
{
  // First, we initialize the underlying HE library.
  // We use one of the supplied preset configurations.
  // This performs all the required setup.
  // NOTE: this also generates a public-private key pair,
  // so it's ready now for use.
  shared_ptr<HeContext> hePtr = HelibContext::create(SECURE_CKKS_8192);

  // The SECURE_CKKS_8192 preset is a configuration where
  // Each ciphertext has 8192 slots, i.e., it can hold 8192 numbers.
  // In CKKS, each number can be a complex number.
  // IMPORTANT: Even though the preset name contains the word 'SECURE' it's
  // always required to test that the resulting security matches your needs.
  // Security levels may vary with different library versions.
  always_assert(hePtr->getSecurityLevel() >= 128);

  // This will print the details of the underlying scheme:
  // name, configuration params, and security level.
  cout << "Using scheme: " << endl;
  hePtr->printSignature();

  // we'll send our initialized context as input to this run()
  // function to do some work with it.
  tut_1_run(*hePtr);
}

void tut_1_run(HeContext& he)
{

  // Note that this function receives a class of type HeContext.
  // It's an abstract class that HelibCkksContext is one of the many classes
  // that inherit from it.
  // This allows writing run() in a scheme oblivious way.
  // Later we'll show how this is useful.

  // Let's create our first ciphertext.
  // We'll first create a vector of doubles (1,2,3).
  vector<double> vals1{1.0, 2.0, 3.0};

  // To encrypt it, we need an encoder . . .
  Encoder encoder(he);

  // And a CTile object - this is our ciphertext object.
  CTile c1(he);
  // We'll now encrypt vals1 into c:
  // In HE encryption actually involves two steps: encode, then encrypt.
  // The following method does both.
  encoder.encodeEncrypt(c1, vals1);

  // c1 now contains the vector (1,2,3) encrypted. Since our library was
  // configured
  // to work with 8192 slots, it actually contains (1,2,3,0,0,0...0), with 0-s
  // filling all unused slots.

  // now let's create a second ciphertext:
  vector<double> vals2{4.0, 5.0, 6.0};
  CTile c2(he);
  encoder.encodeEncrypt(c2, vals2);

  // now let's add them together:
  c1.add(c2);

  // c1 is now the encryption of (5,7,9,0,0,0,...), resulting from adding
  // (4,5,6,...) to (1,2,3,...).
  // Actually, since the underlying scheme is CKKS, c1 is only approximately
  // this result.
  // I.e, instead of 5 we may get 5.0000001.

  // Here are three ways to verify it.
  // First, lets simply decrypt and see what we get.
  // This also actually involves two steps: decrypt and decode, done by the
  // following function.
  // Also, some underlying schemes don't work directly with
  // doubles.
  // The one we chose, actually encrypts complex numbers.
  // Since here we only use doubles, we chose the "decryptDecodeDouble" which
  // automatically
  // converts results back to doubles for us.
  vector<double> res = encoder.decryptDecodeDouble(c1);
  // Now let's print the first 4 elements. It should be close to 5,7,9,0.
  cout << "After add:" << endl;
  for (int i = 0; i < 4; ++i)
    cout << "Result[" << i << "]=" << res[i] << endl;

  // And here's a second way to verify it, useful for debugging, or for
  // client-side tests.
  // We rely here on the fact our HeContext object also has the secret
  // (decryption) key.
  // This is useful in the development stage, but in production it will only be
  // available on client
  // side (the client that generated the keys). On the server side, in
  // production, this method
  // won't work.
  vector<double> expectedRes{5, 7, 9};
  encoder.assertEquals(c1, "test result after add", expectedRes, 1e-5);
  // The above call verified c1 contains 5,7, and  9 in the first 3 slots
  // up-to precision of 1e-5. If not, an exception will be thrown.
  cout << "\nAdd worked correctly!" << endl;

  // And here's a third method relying access to the secret key.
  // this will print the first 4 slots (as complex numbers) + the last slot +
  // possibly some additional meta data of this ciphertext (depending on scheme)
  c1.debugPrint("after add");

  // Let's now quickly review more operators you can apply to ciphertexts.

  // Copy c2 to c3.
  CTile c3(c2);

  // Rotate c3 by offset of -2. Each value moves two slots to the right.
  // It should contain values close to (0,0,4,5,6,0,0,...)
  c3.rotate(-2);
  encoder.assertEquals(c3,
                       "test result after rotate",
                       vector<double>{0, 0, 4, 5, 6, 0, 0},
                       1e-5);
  cout << "\nRotate worked correctly!" << endl;
  // print first 8 elements
  c3.debugPrint("after rotate", 8);

  // c2 will be multiplied by c3, and contain values close to
  // (0,0,24,0,0,0,....)
  c2.multiply(c3);
  encoder.assertEquals(
      c2, "test result after rotate", vector<double>{0, 0, 24, 0, 0}, 1e-5);
  cout << "\nMultiply worked correctly!" << endl;
  c2.debugPrint("after multiply");

  // Let's reencrypt (1,2,3) to c1, and do some more manipulation
  encoder.encodeEncrypt(c1, vals1);
  // c1=~ (1,2,3,0,0,0,...)
  c1.square();
  // c1=~ (1,4,9,0,0,0,...)
  c1.negate();
  // c1=~ (-1,-4,-9,0,0,0,...)

  encoder.assertEquals(
      c1, "Final result", vector<double>{-1, -4, -9, 0, 0}, 1e-5);
  cout << "\nFinal result is correct!" << endl;
  c1.debugPrint("Final result");
}