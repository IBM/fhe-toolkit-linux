# Credit Card Fraud Example

This FHE toolkit example demonstrates a use case in the finance domain as well as demonstrating encrypted machine learning. More specifically, the credit card fraud example demonstrates the process of neural network inference over fully homomorphic encrypted dataset and model. The neural network and dataset determine fraudulent activities based on anonymized transactions. The demonstration is split into a privledged client that has access to unencrypted data and models, and an unprivledged server that only performs homomorphic computation in a completely encrypted fashion. Specific details can be found in the section on  running the demonstration later in this document. 


## Relation to a real use case

The concept of providing fully outsourced, but fully encrypted computation to a cloud provider is a major motivating factor in the feild of FHE. This use case example shows the capability of the toolklit to build such applications. While the client and server are not literally separated (nor demonstrating true remote cloud computation), the concepts generalize.  

With respect to the realism of the use-case, the network we use is based on a network architecture implemented by the Kaggle community (in [Credit Card Fraud Detection using Neural Networks](https://www.kaggle.com/omkarsabnis/credit-card-fraud-detection-using-neural-networks)). The example network used for the sample application was trained using a realistic dataset taken from [Credit Card Fraud Detection](https://www.kaggle.com/mlg-ulb/creditcardfraud?select=creditcard.csv). This dataset contains actual anonymized transactions made by credit card holders from September 2013, and the data set containes labeled about transactions being fraudulent or genuine.

Since this is an educational example, to demonstrate one class of use case possibly with the toolkit, we have opted to  simplify the architecture to demonstrate fast runtime with low memory use. In particular, this is achieved by replacing the activation functions with a square function, which reduced precision from 0.83 to 0.8. We encourage our users to explore the full power of the included example FHE AI library that includes more advanced tools that allow maintaining higher precision as well as better performance. Specifically, if you would like to experiment with the trade off between increasing security and model resources, you can increase security by adjusting the 'm' parameter (line 72) to a larger power of 2. By setting the value to 2^17, the sample applicaiton would reach a security level that is equivalent to a 287 bits symmetric key. Doing so will increase security but will also increase CPU time, memory usage, and I/O time. The full version of this library uses advanced techniques to keep all of these measures low even for higher levels of security, as well as, allow parallelization of the process.

Lastly, with respect to realism and performance, the sample application is designed to processes data in batches of m/4 (the 'm' is the same variable that was specified at line 72 as mentioned previously). E.g., if the demo as provided is configured to use m=2^15, the software will process batches of 8,192 transactions. Working in batches usually provides high throughput but low latency, and are inefficient when there are not enough samples to fill the required batch size. The full version of this library uses advanced techniques to allow efficient, low latency inference for any batch size, irrespective of m.


## Build

    cd /opt/IBM/FHE-Workspace/examples/credit_card_fraud
    cmake .
    make

## Run

Run the ML inference over the encrypted dataset:

    ./credit_card_fraud

Note: this takes about 4 seconds for setup time, then it processes 24 batches of about 500 samples each at a rate of about 1.5 seconds per batch, totaling with 12K samples in less than a minute. It requires less than 1 GB of memory.

Add `--all` command line argument to run all 184 batches (the entire validation set), totaling with 94K samples in about 5 minutes.
Add `--data_dir /path/to/data/dir/` command line argument to make the application read its inputs (plain model, samples and labels files) from a specified directory (default would be to read from the directory where this example resides in).

The outputs are saved to the `credit_card_fraud_output` directory.


The demonstration application comprises two different entities, a client and a server. The components are pseudo-realistic in how they interact during the inference. The client loads a plain model, plain samples and plain labels.
The client is the only component with access to these plain, unencrypted, elements. The model and samples are encrypted using FHE. The encrypted elements are saved in files which are in turn processed by the server component. The server is responsible for loading the encrypted model and samples and then running the encrypted samples through the encrypted model during the inference phase. During the inference phase fully encrypted predictions are made. To complete the loop from client to server, the server then saves the encrypted predictions into files, to be retrieved by the client. The client is then able to decrypt the predictions obtained from the server using the secret key. Thus, it is only the trusted client component that has plaintext access, and all the machine learning inference and data are protected completely. As one can imagine from this simplified example, the concept may one day generalize to a deployment with the inference running on another server in the cloud.  

To demonstrate high quality results after decrypting the predictions, the client is able to analyze and compare the results from the encrypted model to the plaintext labels, as well as calculating the perormance of the model. Performance is evaluated  in terms of accuracy, precision, and recall, etc.

To understand the performance of the approach, note how the process is not done all at once, but is rather divided into batches, after an initial setup. For each batch, the client takes a portion of the dataset to be grouped together and form a batch of samples. This batch of samples goes through the process described above, ending with the client obtaining the decrypted predictions for the current batch. The results of the prediction, and the associated analysis, are updated at the end of each batch invocation. This batching technique is a common pattern when implementing FHE workloads to optimize performance. 
