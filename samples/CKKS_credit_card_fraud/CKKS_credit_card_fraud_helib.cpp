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

#include <iostream>

#include "helayers/hebase/hebase.h"
#include "helayers/hebase/helib/HelibCkksContext.h"
#include "helayers/hebase/helib/HelibConfig.h"
#include "ClientServer.h"

using namespace std;
using namespace helayers;

// define names of files to be used for saving and loading of HE contexts and
// encrypted model
const string outDir = getExamplesOutputDir();
const string clientContext = outDir + "/client_context.bin";
const string serverContext = outDir + "/server_context.bin";

/*
 * define the number of slots in each ciphertext object.
 * a value of 512 slots produces a low security level, just for the demo
 * a value of 16384 slots produces a security level of 88.
 * a value of 32768 slots produces a security level of 287 with a relatively
 * long setup time and big model.
 * */
int numSlots = 512;
//  int numSlots = 16384;
//  int numSlots = 32768;

/*
 * create an HELIB context for both the client and the server, and save contexts
 * into files
 * client context contains a secret key while server context does not
 * */
void createContexts()
{

  cout << "Initalizing HElib . . ." << endl;
  HelibConfig conf;
  conf.m = numSlots * 2 * 2;
  if (numSlots == 16384 || numSlots == 32768) {
    conf.r = 50;
    conf.L = 700;
  } else if (numSlots == 512) {
    conf.r = 52;
    conf.L = 1024;
  } else
    throw runtime_error("configuration not tested for given number of slots");

  HelibCkksContext he;
  he.init(conf);
  he.printSignature(cout);

  cout << "Clearing " << outDir << endl;
  FileUtils::createCleanDir(outDir);
  cout << "Saving client side context to " << clientContext << endl;
  bool withSecretKey = true;
  he.saveToFile(clientContext, withSecretKey); // save client context

  cout << "Saving server side context to " << serverContext << endl;
  withSecretKey = false;
  he.saveToFile(serverContext, withSecretKey); // save server context
}

/*
 * the main logic that creates the HELIB contexts, initializes instances of
 * client and server,
 * and runs the inference process one batch of samples after the other.
 * */
int main(int argc, char** argv)
{
  bool runAll = false;
  string dataDir = getDataSetsDir();

  // read args from cmd
  for (int i = 0; i < argc; ++i) {
    if (std::string(argv[i]) == "--all")
      runAll = true;
    if (std::string(argv[i]) == "--data_dir")
      dataDir = std::string(argv[i + 1]);
  }

  cout << "*** Starting inference demo ***" << endl;

  // creating HELIB context for both client and server, save them to files
  createContexts();

  // init client
  Client client(dataDir);
  client.init();

  // init server
  Server server;
  server.init();

  // go over each batch of samples
  int iterations =
      runAll ? client.getNumBatches() : min(24, client.getNumBatches());
  for (int i = 0; i < iterations; ++i) {

    cout << endl
         << "*** Performing inference on batch " << i + 1 << "/" << iterations
         << " ***" << endl;
    // define names of files to be used to save encrypted batch of samples and
    // their correspondent predictions
    const string encryptedSamplesFile =
        outDir + "/encrypted_batch_samples_" + to_string(i) + ".bin";
    const string encryptedPredictionsFile =
        outDir + "/encrypted_batch_predictions_" + to_string(i) + ".bin";

    // encrypt current batch of samples by client, save to file
    client.encryptAndSaveSamples(i, encryptedSamplesFile);

    // load current batch of encrypted samples by server, predict and save
    // encrypted predictions
    server.processEncryptedSamples(encryptedSamplesFile,
                                   encryptedPredictionsFile);

    // load current batch's predictions by client, decrypt and store
    client.decryptPredictions(encryptedPredictionsFile);

    // analyze the server's predictions so far with respect to the expected
    // labels
    client.assessResults();

    // Print prediciton timing statistics
    HELAYERS_TIMER_PRINT_MEASURE_SUMMARY("model-predict");
  }

  cout << endl << "All done!" << endl << endl;
  // Print overview timing of entire run
  HelayersTimer::printOverview();
}
