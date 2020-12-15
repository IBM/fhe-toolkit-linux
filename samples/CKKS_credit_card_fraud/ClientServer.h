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

class Client
{

  helayers::HeContext& he;

  std::shared_ptr<helayers::TrainingSetPlain> ts;

  std::vector<helayers::DoubleMatrixArray> allPredictions;

  int numBatches;

  int batchSize;

  int currentBatch;

  const std::string& dataDir;

public:
  Client(helayers::HeContext& he, const std::string& dataDir);

  ~Client();

  void init();

  void encryptAndSaveSamples(int batch,
                             const std::string& encryptedSamplesFile) const;

  void decryptPredictions(const std::string& encryptedPredictionsFile);

  void assessResults();

  int getNumBatches() const { return numBatches; }
};

class Server
{

  helayers::HeContext& he;

  std::shared_ptr<helayers::SimpleNeuralNet> encryptedNet;

public:
  Server(helayers::HeContext& he);

  ~Server();

  void init();

  void processEncryptedSamples(
      const std::string& encryptedSamplesFile,
      const std::string& encryptedPredictionsFile) const;
};

#endif /* EXAMPLES_NNFRAUD_CLIENTSERVER_H */
