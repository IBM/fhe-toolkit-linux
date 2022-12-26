---
# IBM Fully Homomorphic Encryption Toolkit for Linux

The IBM Fully Homomorphic Encryption (FHE) Toolkit for Linux is packaged as [Docker][1] containers that make it easier to get started and experimenting with the [Fully Homomorphic Encryption][2] technology.

This repository contains all the scripts required to install and run the pre-packaged toolkits as a docker container. The toolkits comprise the [IBM Homomorphic Encryption Library - HElib][3], an [Integrated Development Environment (IDE)][4], and the corresponding workspace containing the ready-to-run example code in a variety of supported toolkit editions. The editions supported are based on [Centos][5], [Fedora][6], or [Ubuntu][7], or [Alpine][8]. Each toolkit edition provides access to the built-in IDE via a web browser on your host. For the more adventurous users, instructions to build and package the toolkit locally are also provided.

If you are instead looking for the IBM Fully Homomorphic Encryption Toolkit for macOS/iOS that provide a native toolkit for Apple developers, it can be found <a href="https://github.com/IBM/fhe-toolkit-macos" target="_blank">here</a>.

[![Fully Homomorphic Encryption](http://img.youtube.com/vi/5Mhbaeuv5fk/0.jpg)](http://www.youtube.com/watch?v=5Mhbaeuv5fk "Intro to FHE")

To learn more about FHE in general, and what it can be used for, you can check out our [FAQ/Content Solutions page][13].


## Supported Configurations

At this time, the toolkits support many <a href="https://www.docker.com/resources/what-container" target="_blank">Docker</a> capable hosts such as most modern Linux distributions, macOS, <a href="https://docs.microsoft.com/en-us/windows/wsl/install-win10" target="_blank">Windows 10 Subsystem for Linux</a> and <a href="https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.4.0/com.ibm.zos.v2r4.izso100/izso100_whatisintro.htm" target="_blank">z/OS Container Extensions</a>. Other host operating systems with recent Docker software may work as well but are untested. On x86-64 based systems, we support all three operating system editions of the toolkit. For the s390x architecture, the toolkit presently supports the Ubuntu and Fedora Editions. We hope to offer other s390x operating system options for the toolkits soon. 

## Need Help to Get Started?

Corporate clients can email fhestart@us.ibm.com to request a design thinking workshop about building FHE solutions for your business use cases at no cost for applicants accepted to our sponsor user program. For corporate support outside of our sponsor user program, <a href="https://www.ibm.com/security/services/homomorphic-encryption" target="_blank">IBM Security Homomorphic Encryption Services</a> can help unlock the value of your sensitive data without decrypting it to help maintain your privacy and compliance controls. Our trusted advisors offer commercial education, expert support and testing environments to build your prototype applications. 


## Running the Toolkit

If you want to dive right in and get started installing a pre-built container available from [Docker Hub][9] and using the pre-configured IDE Workspace, please see the [Getting Started Document](GettingStarted.md). 

## Building the Toolkit

If you want to build and deploy the toolkit from scratch and perhaps customize the container to your needs, please see the [Getting Started Advanced Document](GettingStarted.Advanced.md).

## Documentation

If you are looking for Documentation to our APIs or more information about individual classes or methods, that can be found here 

* [ML-HElib API Docs](https://ibm.github.io/fhe-toolkit-linux/)
* [HELib API Docs](https://ibm.github.io/fhe-toolkit-linux/html/helib/index.html)

## Deploying to IBM Hyper Protect

If you are looking to build and deploy your FHE image to Hyper Protect Virtual Server, please see [Deploy To Hyper Protect Document](https://github.com/IBM/fhe-toolkit-linux/blob/master/automation/DeployToHyperProtect.md)


## Feedback Survey
 
"IBM invites you to participate in our Advanced Security and Encryption Survey. This survey is designed to gather insights around the security challenges you or your organization face and better understand our users and serve you better. We will only use your feedback to improve the FHE Toolkit experience and inform future IBM security-focused products and services. IBM will not share your response data with any third parties. We look forward to your valuable feedback to improve the IBM Fully Homomorphic Encryption Toolkit for macOS, iOS, and Linux."

https://www.surveygizmo.com/s3/5731822/Advanced-Security-And-Encryption-Survey-2020


## About this distribution

The remainder of this README file explains how the source code and project are organized for those who might want to contribute (also read [contributing guidelines](CONTRIBUTING.md)) to, or modify, the toolkit.  

This toolkit installs the IDE and extensions required to compile, build, and run the included HElib based examples and any application you choose to write to exploit the homomorphic encryption technology. The end result is a containerized environment complete with the source code editor, making it easy for you to develop applications using HElib.

To preserve user privacy, automatic telemetry, and data collection by the IDE have been disabled in this distribution.

Docker is required to build the containerized toolkit. If you are new to Docker, we recommend starting with [Docker Desktop][10]. 

## Source Code Overview

This repository contains a set of Dockerfiles, scripts, and IDE configuration files to run the IBM FHE Toolkit for Linux. The toolkit runs in a Linux docker container and uses the IDE as a workspace and development environment accessible through a browser in your host system.

The toolkit is built from one of the following parent images depending on the platform specified to the setup script:

- fedora:32
- centos:8
- ubuntu:20.04
- alpine:3.12

The same setup script also downloads [HElib][3] and a required dependency Number Theory Library ([NTL][11]). These are stored in `<directory where you cloned the toolkit>/DEPENDENCIES` in the host machine. 

The HElib and NTL distributions are built and installed globally in `/usr/local` as a shared library in the toolkit docker container making them available to include in your program. HElib and NTL also depend on the [GNU Multiple Precision Arithmetic Library (GMP)][12], which is pre-installed when building the docker container.

The demo application source code is copied from `HElib/examples` into a workspace directory `/opt/IBM/FHE-Workspace`. The HElib source distribution can be found in `/opt/IBM/FHE-distro/HElib`. Both may be accessed through the IDE workspace running in the toolkit container, accessed through the web browser on your desktop. Always use the workspace when trying to work with any of these components.   


   [1]: https://www.docker.com/                                  "Docker Container"
   [2]: https://en.wikipedia.org/wiki/Homomorphic_encryption     "Homomorphic Encryption"
   [3]: https://github.com/IBM-HElib/HElib/                         "HElib"
   [4]: https://code.visualstudio.com/                           "Visual Studio Code"
   [5]: https://www.centos.org/                                  "CentOS"
   [6]: https://getfedora.org/                                   "Fedora"
   [7]: https://ubuntu.com/                                      "Ubuntu"
   [8]: https://alpinelinux.org/                                 "Alpine"
   [9]: https://hub.docker.com/u/ibmcom                          "Docker Hub IBM"
   [10]: https://www.docker.com/get-started/                      "Docker get started"
   [11]: https://www.shoup.net/ntl/                               "NTL"
   [12]: https://gmplib.org/                                     "GMP library"
   [13]: https://www.ibm.com/support/z-content-solutions/fully-homomorphic-encryption/ "IBM FHE Content Solutions" 

