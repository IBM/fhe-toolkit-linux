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
#include <iomanip>

#include "ClientServer.h"
#include "helayers/simple_nn/SimpleNeuralNetPlain.h"
#include "helayers/simple_nn/SimpleNeuralNet.h"
#include "helayers/simple_nn/CipherMatrixEncoder.h"

using namespace std;
using namespace helayers;

const string outDir = getExamplesOutputDir();
const string clientContext = outDir + "/client_context.bin";
const string serverContext = outDir + "/server_context.bin";
const string encryptedModelFile = outDir + "/encrypted_model.bin";

// paths from which to load the plain model, samples and labels
const string plainModelFile = "/net_fraud/model_42098.h5";
const string plainSamplesFile = "/net_fraud/x_test.h5";
const string plainLabelsFile = "/net_fraud/y_test.h5";

double classificationThreshold =
    0.5; // used to separate positive from negative samples

// Client methods

Client::Client(const string& dataDir) : currentBatch(0), dataDir(dataDir) {}

Client::~Client() {}

void Client::init()
{
  cout << "CLIENT: loading client side context . . ." << endl;
  he = HeContext::loadHeContextFromFile(clientContext);
  he->printSignature(cout);
  batchSize = he->slotCount();

  cout << "CLIENT: loading plain model . . ." << endl;

  SimpleNeuralNetPlain plainNet;
  H5Parser parser(dataDir + plainModelFile);
  plainNet.loadh5(parser,
                  std::vector<string>{"dense_1", "dense_2", "dense_3"},
                  std::vector<int>{29, 20, 5, 1},
                  batchSize);

  cout << "CLIENT: encrypting plain model . . ." << endl;
  SimpleNeuralNet netHe(*he);
  netHe.initFromNet(plainNet);

  cout << "CLIENT: saving encrypted model . . ." << endl;
  ofstream ofs(encryptedModelFile, ios::out | ios::binary);
  netHe.save(ofs);
  ofs.close();

  cout << "CLIENT: loading plain samples . . ." << endl;
  TrainingSetPlain tmpTs(batchSize);
  tmpTs.loadFromH5(dataDir + plainSamplesFile,
                   "x_test",
                   dataDir + plainLabelsFile,
                   "y_test");
  numBatches = tmpTs.getNumBatches();
  ts = make_shared<TrainingSetPlain>(tmpTs);

  cout << "Number of samples: " << ts->getNumSamples() << endl;
  cout << "Batch size: " << batchSize << endl;
  cout << "Number of batches: " << numBatches << endl;
}

void Client::encryptAndSaveSamples(int batch,
                                   const string& encryptedSamplesFile) const
{
  const CipherMatrixEncoder encoder(*he);

  cout << "CLIENT: encrypting plain samples . . ." << endl;
  HELAYERS_TIMER_PUSH("data-encrypt");
  const DoubleMatrixArray plainSamples = ts->getSamples(batch);
  CipherMatrix encryptedSamples(*he);
  encoder.encodeEncrypt(encryptedSamples, plainSamples.getTensor());
  HELAYERS_TIMER_POP();

  cout << "CLIENT: saving encrypted samples . . ." << endl;
  ofstream ofs(encryptedSamplesFile, ios::out | ios::binary);
  encryptedSamples.save(ofs);
  ofs.close();
}

void Client::decryptPredictions(const string& encryptedPredictionsFile)
{
  CipherMatrixEncoder encoder(*he);
  encoder.getEncoder().setDecryptAddedNoiseEnabled(false);
  cout << "CLIENT: loading encrypted predictions . . ." << endl;

  CipherMatrix encryptedPredictions(*he);
  ifstream ifs(encryptedPredictionsFile, ios::in | ios::binary);
  encryptedPredictions.load(ifs);
  ifs.close();

  cout << "CLIENT: decrypting predictions . . ." << endl;
  HELAYERS_TIMER_PUSH("data-decrypt");
  DoubleMatrixArray plainPredictions(
      encoder.decryptDecodeDouble(encryptedPredictions));
  allPredictions.push_back(plainPredictions);
  HELAYERS_TIMER_POP();
}

void Client::assessResults()
{
  cout << "CLIENT: assessing results so far . . ." << endl;

  int truePositives = 0;
  int trueNegatives = 0;
  int falsePositives = 0;
  int falseNegatives = 0;
  currentBatch++;

  // go over each batch and count hits
  for (int i = 0; i < currentBatch; ++i) {

    const DoubleMatrixArray labels = ts->getLabels(i);
    const DoubleMatrixArray predictions = allPredictions.at(i);

    size_t samplesToCheck = labels.size();
    if (i == numBatches - 1) // last batch may partially be populated with
                             // "dummy" labels to ignore
      samplesToCheck = ts->getNumSamples() - (batchSize * (numBatches - 1));

    for (int j = 0; j < samplesToCheck; ++j) {
      int label = labels.getMat(j).get(0, 0);
      int classification =
          (predictions.getMat(j).get(0, 0) > classificationThreshold ? 1 : 0);

      if (classification == label && classification == 1)
        truePositives++;
      else if (classification == label && classification == 0)
        trueNegatives++;
      else if (classification != label && classification == 1)
        falsePositives++;
      else
        falseNegatives++;
    }
  }

  double precision = ((double)truePositives / (truePositives + falsePositives));
  double recall = ((double)truePositives / (truePositives + falseNegatives));
  double f1Score = (2 * precision * recall) / (precision + recall);

  cout << endl;
  cout << "|---------------------------------------------|" << endl;
  cout << "|                       |    True condition   |" << endl;
  cout << "|                       ----------------------|" << endl;
  cout << "|                       | Positive | Negative |" << endl;
  cout << "|---------------------------------------------|" << endl;
  cout << "| Predicted  | Positive |" << setw(8) << truePositives << "  |"
       << setw(8) << falsePositives << "  |" << endl;
  cout << "|            |--------------------------------|" << endl;
  cout << "| condition  | Negative |" << setw(8) << falseNegatives << "  |"
       << setw(8) << trueNegatives << "  |" << endl;
  cout << "|---------------------------------------------|" << endl;
  cout << endl;
  cout << "Precision: " << precision << endl;
  cout << "Recall: " << recall << endl;
  cout << "F1 score: " << f1Score << endl;
}

// Server methods
Server::~Server() {}

void Server::init()
{
  cout << "SERVER: loading server side context . . ." << endl;
  he = HeContext::loadHeContextFromFile(serverContext);
  he->printSignature(cout);

  cout << "SERVER: loading encrypted model . . ." << endl;
  ifstream ifs(encryptedModelFile, ios::in | ios::binary);
  SimpleNeuralNet net(*he);
  net.load(ifs);
  ifs.close();

  encryptedNet = make_shared<SimpleNeuralNet>(net);
}

void Server::processEncryptedSamples(
    const string& encryptedSamplesFile,
    const string& encryptedPredictionsFile) const
{
  const CipherMatrixEncoder encoder(*he);

  cout << "SERVER: loading encrypted samples . . ." << endl;

  CipherMatrix encryptedSamples(*he);
  ifstream ifs(encryptedSamplesFile, ios::in | ios::binary);
  encryptedSamples.load(ifs);
  ifs.close();

  cout << "SERVER: predicting over encrypted samples . . ." << endl;
  CipherMatrix encryptedPredictions(*he);
  encryptedNet->predict(encryptedSamples, encryptedPredictions);

  cout << "SERVER: saving encrypted predictions . . ." << endl;
  ofstream ofs(encryptedPredictionsFile, ios::out | ios::binary);
  encryptedPredictions.save(ofs);
  ofs.close();
}
