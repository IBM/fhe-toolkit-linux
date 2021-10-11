---
# IBM Fully Homomorphic Encryption (HELayers) SDK for Linux

![HELayers](Documentation/Images/fhe.jpg)

Last year, we introduced FHE to Linux with our FHE Toolkit. Today, we are announcing the next evolution of the FHE Toolkit called IBM HElayers, a software development kit (SDK) for the practical and efficient execution of encrypted workloads using fully homomorphic encrypted data. HElayers enables application developers and data scientists to seamlessly use advanced privacy preserving techniques without having to be a specialist in cryptography - all while working in a newly integrated Python environment. 

HELayers is packaged as Docker containers that make it easier to get started and experimenting with FHE.  It is written in C++ and includes a Python API that enables data scientists and application developers to easily use the power of FHE by supporting a wide array of analytics such as linear regression, logistic regression, and neural networks.  

It is delivered as an open platform that is capable of using the latest FHE schemes and libraries for a given use case. HElayers currently ships with 15 tutorials and sample applications that highlight the basics of FHE and how to use this technology in a practical way. Sample applications include credit card fraud detection, encrypted database search, heart disease detection, image classification and more.  

In this repository is a script that you can run to install and run the pre-packaged SDKs, cpp or python, as a docker container.

     ./StartHELayers.sh 

If you are instead looking for the IBM Fully Homomorphic Encryption Toolkit for Linux README, it can be found [here](README_TOOLKIT.md).


### Try it for yourself

To run the script in this reposistory, clone the repo, open a Terminal and navigate to the root of this repo (`cd fhe-toolkit-linux/`).  

To run the Python HELayers:

     ./StartHELayers.sh python

For the C++ version:

     ./StartHELayers.sh cpp

The script will pull & download the latest version that works on your machine's architecture. It will run locally in its own container for you to view in your web browser.  The script will tell you where to point your browser to in the Terminal after completing the setup. The c++ version uses VSCode integrated into the browser for an IDE, using port 8443, and the python relies on a self-contained jupyter notebook on port 8888.

To try HELayers out from Dockerhub directly, you can use these links below:

* Python
   * [HELayers Python x86](https://hub.docker.com/r/ibmcom/helayers-pylab)
   * [HELayers Python s390x](https://hub.docker.com/r/ibmcom/helayers-pylab-s390x)
* C++
   * [HELayers C++ x86](https://hub.docker.com/r/ibmcom/helayers-lab)
   * [HELayers C++ s390x](https://hub.docker.com/r/ibmcom/helayers-lab-s390x)

### Take a look at our in-depth walkthrough video on how to download and get started with HElayers.

[![Fully Homomorphic Encryption](http://img.youtube.com/vi/_bEMWffloas/0.jpg)](https://www.youtube.com/watch?v=_bEMWffloas "Getting Started with HELayers")

To learn more about FHE in general, and what it can be used for, you can check out our [FAQ/Content Solutions page](https://www.ibm.com/support/z-content-solutions/fully-homomorphic-encryption/ ).

## System Requirements

HElayers is a Linux based Docker container and can be implemented entirely in software; there is no hardware dependency. The only software that is required is Docker version 19 or higher with the necessary user privileges to run `docker` commands. It is also assumed you have a working internet connection. 

Runtimes tested include Linux, Linux on IBM Z (z15 and LinuxONE III and later), z/OS Container Extensions (v2.4 & v2.5), Windows 10 subsystem for Linux and MacOS. Today, the only distribution we support is Ubuntu 20.04 which is included as the base image. 

Older versions have not been thoroughly tested and there are no plans to support older versions in the future.


## License

This image is provided under a community edition license for non-commercial use. Customers who want to work directly with IBM Research, access advanced features, and plan for commercial-grade deployment using HElayers can engage through the Premium Edition Program by contacting the IBM FHE team at FHEstart@us.ibm.com.

## Feedback Survey

HELayers is the next step in the advancement of FHE at IBM.  If you are moving from the Toolkit, you will see familiar code that helped get your applications up and running built around the HElib library.  We have extended that and now offer much more in this edition.  We invite all the Toolkit users to try out HELayers and let us know what you think by taking the survey below and providing feedback on how we can improve and serve you better.  We will only use your feedback to improve the HELayers experience and inform future IBM security-focused products and services.  IBM will not share your response data with any third parties.  We look forward to your valuable feedback to improve the IBM FHE experience. 

[https://www.surveygizmo.com/s3/6494169/IBM-HElayers-SDK-Survey](https://www.surveygizmo.com/s3/6494169/IBM-HElayers-SDK-Survey)


