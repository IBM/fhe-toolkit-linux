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

#include "helayers/hebase/helib/HelibBgvContext.h"
#include "helayers/hebase/hebase.h"

// In this tutorial we'll use a different scheme.
// So far we used CKKS that works with complex numbers,
// Here we'll configure HElib's BGV as the underlyin scheme,
// which works with integer numbers.
// Hence it has some limitations, and some
// additional operators.

using namespace std;
using namespace helayers;

void tut_4_run(HeContext& he);

void tut_4_integers()
{
  // First, we initialize the library
  // As before, this performs all the required setup.
  // NOTE: this also generates a public-private key pair,
  // so it's ready now for use.
  int p = 4999;  // Plaintext prime modulus
  int r = 1;     // Hensel lifting
  int m = 32109; // Cyclotomic polynomial - defines \phi(m)
  int L = 500;   // Max circuit depth
  HelibBgvContext helib;
  helib.init(p, m, r, L);

  // This will print the details of the underlying scheme:
  // name, configuration params, and security level.
  cout << "Using scheme: " << endl;
  helib.printSignature();

  // This configuration is actually not secure.
  // But we use it only for demo purposes.
  // always_assert(helib.getSecurityLevel() >= 128);

  // we'll send our initialized context as input to this run()
  // function to do some work with it.
  tut_4_run(helib);
}

void tut_4_run(HeContext& he)
{
  // Note that this function receives a class of type HeContext.
  // It's an abstract class that HelibBgvContext is one of the many classes
  // that inherit from it.
  // This allows writing run() in a scheme oblivious way.

  // Let's create our first ciphertext.
  // We'll first create a vector of integers (1,2,3).
  vector<int> vals1{1, 2, 3};

  // To encrypt it, we need an encoder . . .
  Encoder encoder(he);

  // And a CTile object - this is our ciphertext object.
  CTile c1(he);

  // We'll now encrypt vals1 into c:
  // In HE encryption actually involves two steps: encode, then encrypt.
  // The following method does both.
  encoder.encodeEncrypt(c1, vals1);

  // Each ciphertext has slotCount slots where each operation is applied in SIMD
  int slotCount = he.slotCount();
  cout << "Each ciphertext has " << slotCount << " slots" << endl;

  // c1 now contains the vector (1,2,3) encrypted. Since our library was
  // configured
  // to work with slotCount slots, it actually contains (1,2,3,0,0,0...0), with
  // 0-s
  // filling all unused slots.

  // now let's create a second ciphertext:
  vector<int> vals2{4, 5, 6};
  CTile c2(he);
  encoder.encodeEncrypt(c2, vals2);

  // now let's add two ciphertexts
  CTile c_sum(he);
  c_sum = c1;
  c_sum.add(c2);

  // Let's decrypt and check
  // Decryption involves two steps: decryption and decoding. This function does
  // both.
  vector<int> p_sum = encoder.decryptDecodeInt(c_sum);
  cout << "Adding two ciphertexts:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_sum[i] << " = " << vals1[i] << " + " << vals2[i] << endl;
  }
  cout << endl;

  // If we have a vector of plaintext we don't need to encrypt it.
  // It is enough to encode it and add it as a plaintext.
  CTile c_sum_plaintext(he);
  c_sum_plaintext = c1;
  PTile p2(he);
  encoder.encode(p2, vals2);
  c_sum_plaintext.addPlain(p2);
  vector<int> p_sum_plaintext = encoder.decryptDecodeInt(c_sum_plaintext);
  cout << "adding a ciphertext with a plaintext:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_sum_plaintext[i] << " = " << vals1[i] << " + " << vals2[i]
         << endl;
  }
  cout << endl;

  // If we want to add the same scalar to each slot in the vector we can use
  // addScalar.
  CTile c_add_scalar(he);
  c_add_scalar = c1;
  c_add_scalar.addScalar(1);
  vector<int> p_add_scalar = encoder.decryptDecodeInt(c_add_scalar);
  cout << "adding a ciphertext with a scalar:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_add_scalar[i] << " = " << vals1[i] << " + 1" << endl;
  }
  cout << endl;

  // Homomorphic encryption also supports multiplying two ciphertexts.
  // That multiplies the two vectors slotwise
  CTile c_product(he);
  c_product = c1;
  c_product.multiply(c2);
  vector<int> p_product = encoder.decryptDecodeInt(c_product);
  cout << "multiplying two ciphertext:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_product[i] << " = " << vals1[i] << " * " << vals2[i] << endl;
  }
  cout << endl;

  // If we have an array of plaintext numbers it is enough to only encode the
  // plaintext
  CTile c_product_plaintext(he);
  c_product_plaintext = c1;
  c_product_plaintext.multiplyPlain(p2);
  vector<int> p_product_plaintext =
      encoder.decryptDecodeInt(c_product_plaintext);
  cout << "multiplying a ciphertext with a plaintext:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_product_plaintext[i] << " = " << vals1[i] << " * " << vals2[i]
         << endl;
  }
  cout << endl;

  // We can also multiply a ciphertext by a scalar. In that case each slot in
  // the ciphertext
  // is multiplied by the same scalar
  CTile c_product_scalar(he);
  c_product_scalar = c1;
  c_product_scalar.multiplyScalar(2);
  vector<int> p_product_scalar = encoder.decryptDecodeInt(c_product_scalar);
  cout << "multiplying a ciphertext with a scalar:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_product_scalar[i] << " = " << vals1[i] << " * 2" << endl;
  }
  cout << endl;

  // A special property of this scheme is that it
  // Works using modular arithmetic, with the modulus
  // Given by the p we initialized it with.
  // We also obtain it again using the API:
  long p = he.getTraits().getArithmeticModulus();

  // We can use this property to compute the
  // the multiplicative inverse of a ciphertext.
  // (That is x^-1 for each x in each slot in the ciphertext)
  // To do this we'll use Fermat's
  // Little Theorem.  We calculate a^{-1} = a^{p-2} mod p, where a is non-zero
  // and p is our modulus.
  // First make a copy of the ctxt using copy constructor
  CTile c_inverse(c1);
  // to compute power we'll use this helper class:
  NativeFunctionEvaluator eval(he);
  eval.powerInPlace(c_inverse, p - 2);
  vector<int> p_inverse = encoder.decryptDecodeInt(c_inverse);
  cout << "multiplicative inverse:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_inverse[i] << " = " << vals1[i] << "^-1" << endl;
  }
  cout << endl;

  // Multiplicative inverse can be used for modular division.
  // We'll now use the inverse we computed to divie the ciphertext with itself.
  // Multiplying the original ciphertext with its inverse, is equivalent to
  // dividing it by itself, and should result with 1.
  CTile c_divRes(c1);
  // to compute power we'll use this helper class:
  c_divRes.multiply(c_inverse);
  vector<int> p_divRes = encoder.decryptDecodeInt(c_divRes);
  cout << "Division result:" << endl;
  for (int i = 0; i < vals1.size(); ++i) {
    cout << p_divRes[i] << " = " << vals1[i] << " / " << vals1[i] << endl;
  }
  cout << endl;
}