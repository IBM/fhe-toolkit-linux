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

// This is a sample program for education purposes only.
// It implements a very simple homomorphic encryption based
// db search algorithm for demonstration purposes.

// This country lookup example is derived from the BGV database demo
// code originally writte by Jack Crawford for a lunch and learn
// session at IBM Research (Hursley) in 2019.
// The original example code ships with HElib and can be found at
// https://github.com/homenc/HElib/tree/master/examples/BGV_database_lookup

#include <iostream>

#include "helayers/hebase/hebase.h"
#include "helayers/hebase/helib/HelibBgvContext.h"
#include <fstream>
#include <helib/ArgMap.h>
#include <NTL/BasicThreadPool.h>
#include <cassert>

using namespace helayers;
using namespace std;

// Forward declarations. These functions are explained later.
vector<pair<string, string>> read_csv(string filename, int maxLen);
void run(HeContext& he, const string& db_filename, bool debug);
bool isPowerOf2(int v);
vector<int> stringToAscii(const string& val);

int main(int argc, char* argv[])
{
  // Note: The parameters have been chosen to provide a somewhat
  // faster running time with a non-realistic security level.
  // Do Not use these parameters in real applications.

  // Plaintext prime modulus
  unsigned long p = 127;
  // Cyclotomic polynomial - defines phi(m)
  unsigned long m = 128; // this will give 32 slots
  // Hensel lifting (default = 1)
  unsigned long r = 1;
  // Number of bits of the modulus chain
  unsigned long bits = 1000;
  // Number of columns of Key-Switching matrix (default = 2 or 3)
  unsigned long c = 2;
  // Size of NTL thread pool (default =1)
  unsigned long nthreads = 1;
  // input database file name
  string db_filename =
      getExamplesDir() + "/BGV_world_country_db_lookup/countries_dataset.csv";
  // debug output (default no debug output)
  bool debug = false;

  helib::ArgMap amap;
  amap.arg("m", m, "Cyclotomic polynomial ring");
  amap.arg("p", p, "Plaintext prime modulus");
  amap.arg("r", r, "Hensel lifting");
  amap.arg("bits", bits, "# of bits in the modulus chain");
  amap.arg("c", c, "# fo columns of Key-Switching matrix");
  amap.arg("nthreads", nthreads, "Size of NTL thread pool");
  amap.arg(
      "db_filename", db_filename, "Qualified name for the database filename");
  amap.toggle().arg("-debug", debug, "Toggle debug output", "");
  amap.parse(argc, argv);

  // set NTL Thread pool size
  if (nthreads > 1)
    NTL::SetNumThreads(nthreads);

  cout << "\n*********************************************************";
  cout << "\n*           Privacy Preserving Search Example           *";
  cout << "\n*           =================================           *";
  cout << "\n*                                                       *";
  cout << "\n* This is a sample program for education purposes only. *";
  cout << "\n* It implements a very simple homomorphic encryption    *";
  cout << "\n* based db search algorithm for demonstration purposes. *";
  cout << "\n*                                                       *";
  cout << "\n*********************************************************";
  cout << endl;

  cout << "---Initialising HE Environment ... ";
  // Initialize context
  cout << "\nInitializing the Context ... " << endl;

  // To setup helib using the hebase layer, let's first
  // copy all configuration params to an HelibConfig object:
  HelibConfig conf;
  conf.p = p;
  conf.m = m;
  conf.r = r;
  conf.L = bits;
  conf.c = c;

  // Next we'll initialize a BGV scheme in helib.
  // The following two lines perform full intializiation
  // Including key generation.
  // (We added code for timing it).
  HELIB_NTIMER_START(timer_Context);
  HelibBgvContext he;
  he.init(conf);
  HELIB_NTIMER_STOP(timer_Context);

  // Helib-BGV is now ready to start doing some HE work.
  // which we'll do in the follwing function, defined below
  run(he, db_filename, debug);

  return 0;
}

void run(HeContext& he, const string& db_filename, bool debug)
{

  // The run function receives an abstract HeContext class.
  // Therefore the code below is oblivious to a particular HE scheme
  // implementation.

  // First let's print general information on our library and scheme.
  // This will print their names, and the configuraton details.
  he.printSignature();

  // However we do have some requirements that we can
  // assert exists:
  // We require the plaintext to be over modular arithmetic.
  // We'll rely on that later.
  assert(he.getTraits().getIsModularArithmetic());
  // Since we store ascii codes, we need it at least to be able
  // to handle the numbers 0...127
  assert(he.getTraits().getArithmeticModulus() >= 127);

  // Next, print the security level
  // Note: This will be negligible to improve performance time.
  cout << "\n***Security Level: " << he.getSecurityLevel()
       << " *** Negligible for this example ***" << endl;

  // Let's also print the number of slots.
  // Each ciphertext will have this many slots.
  cout << "\nNumber of slots: " << he.slotCount() << endl;

  // Now we'll read in the database (in cleartext).
  // This function we'll make sure no string is longer than he.slotCount()
  vector<pair<string, string>> country_db =
      read_csv(db_filename, he.slotCount());

  cout << "\n---Initializing the encrypted key,value pair database ("
       << country_db.size() << " entries)...";
  cout << "\nConverting strings to numeric representation into Ptxt objects ..."
       << endl;

  // We'll now encrypt our country-capital database.
  HELIB_NTIMER_START(timer_CtxtCountryDB);
  // The encoder class handles both encoding and encrypting.
  Encoder enc(he);
  // This is the database: a vector of pairs of CTile-s.
  // A CTile is a ciphertext object.
  vector<pair<CTile, CTile>> encrypted_country_db;
  for (const auto& country_capital_pair : country_db) {
    // Create a country ciphertext, and encrypt inside
    // the ascii vector representation of each country.
    // For example, Norway is represented
    // (78,111,114,119,97,121,  0,0,0, ...)
    CTile country(he);
    enc.encodeEncrypt(country, stringToAscii(country_capital_pair.first));
    // Similarly encrypt the capital name
    CTile capital(he);
    enc.encodeEncrypt(capital, stringToAscii(country_capital_pair.second));
    // Add the pair to the database
    encrypted_country_db.emplace_back(move(country), move(capital));
  }
  HELIB_NTIMER_STOP(timer_CtxtCountryDB);

  if (debug) {
    helib::printNamedTimer(cout << endl, "timer_Context");
    helib::printNamedTimer(cout, "timer_CtxtCountryDB");
  }

  cout << "\nInitialization Completed - Ready for Queries" << endl;
  cout << "--------------------------------------------" << endl;

  /** Create the query **/

  // Read in query from the command line
  string query_string;
  cout << "\nPlease enter the name of a Country: ";

  getline(cin, query_string);

  cout << "Looking for the Capital of " << query_string << endl;
  cout << "This may take a few minutes ... " << endl;

  HELIB_NTIMER_START(timer_TotalQuery);
  HELIB_NTIMER_START(timer_EncryptQuery);

  // Encrypt the query similar to the way we encrypted
  // the country and capital names
  CTile query(he);
  enc.encodeEncrypt(query, stringToAscii(query_string));

  HELIB_NTIMER_STOP(timer_EncryptQuery);

  /************ Perform the database search ************/

  HELIB_NTIMER_START(timer_QuerySearch);
  vector<CTile> mask;
  mask.reserve(country_db.size());
  NativeFunctionEvaluator eval(he);
  long modulusP = he.getTraits().getArithmeticModulus();

  // For every entry in our database we perform the following
  // calculation:
  for (const auto& encrypted_pair : encrypted_country_db) {
    //  Copy of database key: a country name
    CTile mask_entry = encrypted_pair.first;
    // Calculate the difference
    // In each slot now we'll have 0 when characters match,
    // or non-zero when there's a mismatch
    mask_entry.sub(query);

    // Fermat's little theorem:
    // Since the underlying plaintext are in modular arithmetic,
    // Raising to the power of modulusP convers all non-zero values
    // to 1.
    eval.powerInPlace(mask_entry, modulusP - 1);

    // Negate the ciphertext
    // Now we'll have 0 for match, -1 for mismatch
    mask_entry.negate();

    // Add +1
    // Now we'll have 1 for match, 0 for mismatch
    mask_entry.addScalar(1);

    // We'll now multiply all slots together, since
    // we want a complete match across all slots.

    // If slot count is a power of 2 there's an efficient way
    // to do it:
    // we'll do a rotate-and-multiply algorithm, similar to
    // a rotate-and-sum one.
    if (isPowerOf2(he.slotCount())) {
      for (int rot = 1; rot < he.slotCount(); rot *= 2) {
        CTile tmp(mask_entry);
        tmp.rotate(-rot);
        mask_entry.multiply(tmp);
      }
    } else {
      // Otherwise we'll create all possible rotations, and multiply all of
      // them.
      // Note that for non powers of 2 a rotate-and-multiply algorithm
      // can still be used as well, though it's more complicated and
      // beyond the scope of this example.
      vector<CTile> rotated_masks(he.slotCount(), mask_entry);
      for (int i = 1; i < rotated_masks.size(); i++)
        rotated_masks[i].rotate(-i); // Rotate each of the masks
      eval.totalProduct(mask_entry,
                        rotated_masks); // Multiply each of the masks
    }

    // mask_entry is now either all 1s if query==country,
    // or all 0s otherwise.
    // After we multiply by capital name it will be either
    // the capital name, or all 0s.
    mask_entry.multiply(encrypted_pair.second);
    // We collect all our findings.
    mask.push_back(mask_entry);
  }
  HELIB_NTIMER_STOP(timer_QuerySearch);

  // Aggregate the results into a single ciphertext
  // Note: This code is for educational purposes and thus we try to refrain
  // from using the STL and do not use std::accumulate
  CTile value = mask[0];
  for (int i = 1; i < mask.size(); i++)
    value.add(mask[i]);

  // /************ Decrypt and print result ************/

  HELIB_NTIMER_START(timer_DecryptQueryResult);
  vector<int> res = enc.decryptDecodeInt(value);
  HELIB_NTIMER_STOP(timer_DecryptQueryResult);

  // Convert from ASCII to a string
  string string_result;
  for (long i = 0; i < res.size(); ++i)
    string_result.push_back(static_cast<long>(res[i]));

  HELIB_NTIMER_STOP(timer_TotalQuery);

  // Print DB Query Timers
  if (debug) {
    helib::printNamedTimer(cout << endl, "timer_EncryptQuery");
    helib::printNamedTimer(cout, "timer_QuerySearch");
    helib::printNamedTimer(cout, "timer_DecryptQueryResult");
    cout << endl;
  }

  if (string_result.at(0) == 0x00) {
    string_result = "Country name not in the database.\n*** Please make sure "
                    "to enter the name of an European Country\n*** with the "
                    "first letter in upper case.";
  }

  cout << "\nQuery result: " << string_result << endl;
  helib::printNamedTimer(std::cout, "timer_TotalQuery");
}

// Utility function to read <K,V> CSV data from file
vector<pair<string, string>> read_csv(string filename, int maxLen)
{
  vector<pair<string, string>> dataset;
  ifstream data_file(filename);

  if (!data_file.is_open())
    throw runtime_error(
        "Error: This example failed trying to open the data file: " + filename +
        "\n           Please check this file exists and try again.");

  vector<string> row;
  string line, entry, temp;

  if (data_file.good()) {
    // Read each line of file
    while (getline(data_file, line)) {
      row.clear();
      stringstream ss(line);
      while (getline(ss, entry, ',')) {
        row.push_back(entry);
      }
      if (row[0].size() > maxLen)
        throw runtime_error("Country name " + row[0] + " too long");
      if (row[1].size() > maxLen)
        throw runtime_error("Capital name " + row[1] + " too long");

      // Add key value pairs to dataset
      dataset.push_back(make_pair(row[0], row[1]));
    }
  }

  data_file.close();
  return dataset;
}

// Return a vector of ints with the i'th element containing the ascii
// code of the i'th character
vector<int> stringToAscii(const string& val)
{
  vector<int> res;
  res.reserve(val.size());
  for (size_t i = 0; i < val.size(); ++i) {
    res.push_back(val[i]);
  }
  return res;
}

// Returns true if v is a power of 2
bool isPowerOf2(int v) { return (v & v - 1) == 0; }