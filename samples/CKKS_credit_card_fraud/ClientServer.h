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

#ifndef EXAMPLES_NNFRAUD_CLIENTSERVER_H
#define EXAMPLES_NNFRAUD_CLIENTSERVER_H

#include "helayers/hebase/hebase.h"
#include "helayers/simple_nn/SimpleNeuralNet.h"
#include "helayers/simple_nn/TrainingSetPlain.h"

/// A class representing the client side
class Client
{

  std::shared_ptr<helayers::HeContext> he;

  std::shared_ptr<helayers::TrainingSetPlain> ts;

  std::vector<helayers::DoubleMatrixArray> allPredictions;

  int numBatches;

  int batchSize;

  int currentBatch;

  const std::string& dataDir;

public:
  /// Construct a client.
  /// @param[in] dataDir folder where input data is
  Client(const std::string& dataDir);

  ~Client();

  /// Initialize: Load he context, load network, load training set,
  /// Encrypt network and save it to file to be sent to server.
  void init();

  /// Encrypt a batch of samples and save to file to be sent to server.
  /// @param[in] batch Batch number
  /// @param[in] encryptedSamplesFile File name to write to
  void encryptAndSaveSamples(int batch,
                             const std::string& encryptedSamplesFile) const;

  /// Loads a batch of predictions from file, decrypt them, and store results
  /// in a member for assessment.
  /// @param[in] encryptePredictionsFile File name to read from
  void decryptPredictions(const std::string& encryptedPredictionsFile);

  /// Assess received predictions compared with training set's labels (the
  /// ground truth).
  void assessResults();

  /// Total number of batches in training set.
  int getNumBatches() const { return numBatches; }
};

/// A class representing the server side
class Server
{

  std::shared_ptr<helayers::HeContext> he;

  std::shared_ptr<helayers::SimpleNeuralNet> encryptedNet;

public:
  ~Server();

  void init();

  void processEncryptedSamples(
      const std::string& encryptedSamplesFile,
      const std::string& encryptedPredictionsFile) const;
};

#endif /* EXAMPLES_NNFRAUD_CLIENTSERVER_H */
