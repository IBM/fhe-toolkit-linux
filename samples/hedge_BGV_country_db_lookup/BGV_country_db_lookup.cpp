/* Copyright (C) 2020 IBM Corp.
 * This program is Licensed under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. See accompanying LICENSE file.
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

#include "hedge/hebase/hebase.h"
#include "hedge/hebase/helib/HelibBgvContext.h"
#include <fstream>
#include <helib/ArgMap.h>
#include <NTL/BasicThreadPool.h>

using namespace hedge;
using namespace std;

// Utility function to read <K,V> CSV data from file
vector<pair<string, string>> read_csv(string filename)
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
      // Add key value pairs to dataset
      dataset.push_back(make_pair(row[0], row[1]));
    }
  }

  data_file.close();
  return dataset;
}

vector<int> stringToAscii(const string& val)
{
  vector<int> res;
  res.reserve(val.size());
  for (size_t i = 0; i < val.size(); ++i) {
    res.push_back(val[i]);
  }
  return res;
}

int main(int argc, char* argv[])
{
  /************ HElib boiler plate ************/

  // Note: The parameters have been chosen to provide a somewhat
  // faster running time with a non-realistic security level.
  // Do Not use these parameters in real applications.

  // Plaintext prime modulus
  unsigned long p = 131;
  // Cyclotomic polynomial - defines phi(m)
  unsigned long m = 130; // this will give 48 slots
  // Hensel lifting (default = 1)
  unsigned long r = 1;
  // Number of bits of the modulus chain
  unsigned long bits = 1000;
  // Number of columns of Key-Switching matrix (default = 2 or 3)
  unsigned long c = 2;
  // Size of NTL thread pool (default =1)
  unsigned long nthreads = 1;
  // input database file name
  string db_filename = "./countries_dataset.csv";
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
  cout << "\nInitializing the Context ... ";
  HELIB_NTIMER_START(timer_Context);
  HelibConfig conf;
  conf.p = p;
  conf.m = m;
  conf.r = r;
  conf.L = bits;
  conf.c = c;
  HelibBgvContext he;
  he.init(conf);
  HELIB_NTIMER_STOP(timer_Context);

  he.printSignature();

  // Print the security level
  // Note: This will be negligible to improve performance time.
  cout << "\n***Security Level: " << he.getSecurityLevel()
       << " *** Negligible for this example ***" << endl;

  // Get the number of slot (phi(m))
  long nslots = he.slotCount();
  cout << "\nNumber of slots: " << nslots << endl;

  /************ Read in the database ************/
  vector<pair<string, string>> country_db = read_csv(db_filename);

  // Convert strings into numerical vectors
  cout << "\n---Initializing the encrypted key,value pair database ("
       << country_db.size() << " entries)...";
  cout << "\nConverting strings to numeric representation into Ptxt objects ..."
       << endl;

  // Generating the Plain text representation of Country DB
  HELIB_NTIMER_START(timer_CtxtCountryDB);
  Encoder enc(he);
  vector<pair<CTile, CTile>> encrypted_country_db;
  for (const auto& country_capital_pair : country_db) {
    if (debug) {
      cout << "\t\tname_addr_pair.first size = "
           << country_capital_pair.first.size() << " ("
           << country_capital_pair.first << ")"
           << "\tname_addr_pair.second size = "
           << country_capital_pair.second.size() << " ("
           << country_capital_pair.second << ")" << endl;
    }

    CTile country(he);
    enc.encodeEncrypt(country, stringToAscii(country_capital_pair.first));
    CTile capital(he);
    enc.encodeEncrypt(capital, stringToAscii(country_capital_pair.second));
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
  cout << "\nPlease enter the name of an European Country: ";

  getline(cin, query_string);

  cout << "Looking for the Capital of " << query_string << endl;
  cout << "This may take few minutes ... " << endl;

  HELIB_NTIMER_START(timer_TotalQuery);
  HELIB_NTIMER_START(timer_EncryptQuery);

  CTile query(he);
  enc.encodeEncrypt(query, stringToAscii(query_string));

  HELIB_NTIMER_STOP(timer_EncryptQuery);
  /************ Perform the database search ************/

  HELIB_NTIMER_START(timer_QuerySearch);
  vector<CTile> mask;
  mask.reserve(country_db.size());
  NativeFunctionEvaluator eval(he);
  long modulusP = he.getTraits().getArithmeticModulus();
  for (const auto& encrypted_pair : encrypted_country_db) {
    CTile mask_entry = encrypted_pair.first; //  Copy of database key
    mask_entry.sub(query);                   // Calculate the difference

    eval.powerInPlace(mask_entry, modulusP - 1); // Fermat's little theorem
    mask_entry.negate();                         // Negate the ciphertext
    mask_entry.addScalar(1);                     // 1 - mask = 0 or 1

    // Create a vector of copies of the mask
    vector<CTile> rotated_masks(nslots, mask_entry);
    for (int i = 1; i < rotated_masks.size(); i++)
      rotated_masks[i].rotate(-i);                // Rotate each of the masks
    eval.totalProduct(mask_entry, rotated_masks); // Multiply each of the masks

    mask_entry.multiplyRaw(encrypted_pair.second); // multiply mask with values
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

  return 0;
}
