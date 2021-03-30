# FHE Toolkit - Hyper Protect Deployment Automation

[IBM Cloud Hyper Protect Virtual Servers](https://www.ibm.com/cloud/hyper-protect-virtual-servers) (HPVS) make it possible to sign and deploy applications built on the FHE Toolkit into a product-grade hardened environment and runtime.  However, the manual process for deploying the FHE Toolkit and its' derivatives into an HPVS environment are complex and time-consuming.  Automating this process can help to close the technical gaps that prevent enterprise adaptation of production FHE solutions.

<!--
    To update the Table of Contents, you should clone the github-markdown-toc
    repository (https://github.com/ekalinin/github-markdown-toc), and then
    execute the command:
      ./gh-md-toc --insert path/to/HiperProtectExperiments/automation/README.md
-->
## Table of Contents
<!--ts-->
   * [FHE Toolkit - Hyper Protect Deployment Automation](#fhe-toolkit---hyper-protect-deployment-automation)
      * [Table of Contents](#table-of-contents)
      * [What We Have](#what-we-have)
         * [Application Files](#application-files)
         * [Other Important Files](#other-important-files)
         * [BuildRegistrationDefinition](#buildregistrationdefinition)
            * [Prerequisites](#prerequisites)
            * [Installation](#installation)
            * [How To Use It](#how-to-use-it)
         * [DeployToHPVS.sh](#deploytohpvssh)
            * [Prerequisites](#prerequisites-1)
            * [How To Use It](#how-to-use-it-1)
               * [Arguments](#arguments)
                  * [fedora](#fedora)
                  * [alpine](#alpine)
                  * [ubuntu](#ubuntu)
               * [Options](#options)
                  * [-c configFile](#-c-configfile)
                  * [-f configFile](#-f-configfile)
                  * [-h](#-h)
                  * [-l](#-l)
               * [Notes](#notes)
            * [The Configuration File](#the-configuration-file)
               * [Notes](#notes-1)
            * [The Keys](#the-keys)
               * [Automatic key generation](#automatic-key-generation)
               * [Vendor key](#vendor-key)
               * [Delegation keys](#delegation-keys)

<!-- Added by: danfitz, at: Wed Mar  3 12:24:07 EST 2021 -->

<!--te-->

## What We Have

This directory contains scripts and applications that automate the process of deploying a Linux on Z container image to HPVS in the IBM Public Cloud.  While we targeted development for specifically deploying the FHE Toolkit for Linux, they can be used theoretically to deploy any s390x architecture Linux container.

### Application Files

| File | Description |
|-|-|
| `DeployToHPVS.sh` | Sign an IBM Fully Homomorphic Encryption Toolkit container image and deploy it to a new Hyper Protect Virtual Server instance in the IBM public cloud |
| `BuildRegistrationDefinition` (Python 3 module) | Build a Hyper Protect Virtual Server registration definition file (called by `DeployToHPVS.sh`, but can be standalone) |

### Other Important Files

| File | Description |
|-|-|
| `DeployToHPVS.1` | Linux man page for `DeployToHPVS.sh`; invoke with `man ./DeployToHPVS.1` |
| `DeployToHPVS.bashlib` | Library routines for `DeployToHPVS.sh` |
| `DeployToHPVS.conf` | Sample configuration file for `DeployToHPVS.sh` |
| `BuildRegistrationDefinition/README.md` | Documentation for the `BuildRegistrationDefinition` application |
| `Pipfile*` | Software dependencies for the `BuildRegistrationDefinition` application |
| `README.md` | This file |
| `makefile` |  Makefile for building the `BuildRegistrationDefinition` application |
| `setup.py` | Setup module for the `BuildRegistrationDefinition` application |

### `BuildRegistrationDefinition`
This is a stand-alone Python application that builds a HPVS registration file for a given container image.  `BuildRegistrationDefinition` is invoked by `DeployToHPVS.sh` as part of the deployment process, but can be used on its own to automatically generate an HPVS registration file.

#### Prerequisites

 * Python >= 3.6.0
 * Pip3

#### Installation

From the current directory, run:
```
make init
```

The `DeployToHPVS.sh` script will run the `BuildRegistrationDefinition` application without trying to actually build and install it on your system.  If you wish to install the stand-alone application, run:
```
make install
```

There are other makefile targets as well.  For a full list, run:
```
make help
```

#### How To Use It
Please see the [`BuildRegistrationDefinition` program documentation](./BuildRegistrationDefinition/README.md).

### `DeployToHPVS.sh`
Automates the process of signing a Linux on Z container image and deploying it to a new HPVS instance in the IBM public cloud.  In this document, we will assume that you are deploying the FHE Toolkit for Linux image.

#### Prerequisites
 * [Prerequisites for `BuildRegustrationDefinition`](#prerequisites)
 * gpg
 * Signing keys as described [below](#the-keys)
 * An [IBM Cloud](https://cloud.ibm.com) account
 * An [API key](https://tinyurl.com/y529hkwf) for your IBM Cloud account
 * Access to a container registry, such as [DockerHub](https://hub.docker.io) or [IBM Cloud Container Registry](https://www.ibm.com/cloud/container-registry)
 * Docker CE or EE
   * The script requires the `docker trust` command

#### How To Use It
```
                      .-------------.
                      V             |  .-- fedora --.
>>-- DeployToHPVS.sh ---| OPTIONS |-'--+------------+------------------------><
                                       :-- alpine --:
                                       '-- ubuntu --'
```
##### Arguments
###### fedora
###### alpine
###### ubuntu
Name of the s390x FHE Toolkit container variant that you wish to deploy.  If left unspecified, the script defaults to `fedora`.

##### Options
###### -c configFile
Generate a new configuration file at the given path using an interactive wizard.  The new configuration file with the name and path specified.  Mutually exclusive with `-f`.

###### -f configFile
Path to the configuration file for this script.  If left unspecified, the script will use the default file `./DeployToHPVS.conf`, located in the same directory as the script.  Mutually exclusive with `-c`.

###### -h
Display this help information

###### -l
Deploy a locally-built FHE Toolkit container image generated by BuildDockerImage.sh.  If left unspecified, this script will assume that you are deploying a pre-built toolkit fetched from
https://hub.docker.com/u/ibmcom on DockerHub.

##### Notes
1. Only Alpine, Fedora, and Ubuntu are supported as container operating systems as those are the only ones that the FHE Toolkit for Linux project has built for s390x.
2. The default behavior of this script is to attempt to use the ibmcom pre-built toolkit available from Docker Hub.  This behavior can be overridden with the -l option flag.
3. By default, all commands executed by these functions are silenced except for when a nonzero RC is returned.  To display the commands being executed and their output, issue: `export DEPLOY_TO_HPVS_DEBUG=1`.

#### The Configuration File
The behavior of `DeployToHPVS.sh` is controlled by the configuration file.  By default, this is assumed to be a file named `DeployToHPVS.conf` and located in the same directory as the `DeployToHPVS.sh` script itself.  Such a file is provided with this source tree, however it is only a template and should be modified.  We recommend copying the file and making changes to the copy/copies.  If `DeployToHPVS.sh` called with the `-c` option, an interactive wizard will guide you through creating the configuration file and any keys that you require.

The configuration variables are as follows:
| Configuration Variable | Optional? | Description | Default Value |
| - | - | - | - |
| `APIKey` | Y | Value of IBM Cloud API key ("IAM key") | Value of `dockerPW` |
| `DCTServer` | Y | URL of the Docker Content Trust ("notary") server<sup id="DCTServer">[1](#f1)</sup> | Unspecified (`docker trust` will default to using `https://notary.docker.io`) |
| `delegationkey` | Y | Set to `true` if a delegation key is to be used<sup id="delegationkey">[2](#f2)</sup> | `false` |
| `delegationkeyName` | Y | Name of the delegation key<sup id="delegationkeyName">[2](#f2)</sup> | Unspecified |
| `delegationPassphrase` | Y | Delegation key passphrase<sup id="delegationPassphrase">[2](#f2)</sup> | Unspecified |
| `delegationPriFile` | Y | Path to file containing private key value of delegation key<sup id="delegationPriFile">[2](#f2), [3](#f3)</sup> | Unspecified |
| `delegationPubFile` | Y | Path to file containing public key value of delegation key<sup id="delegationPubFile">[2](#f2)</sup> | Unspecified |
| `dockerPW` | | Password to login to the container registry<sup id="dockerPW">[4](#f4)</sup> | |
| `dockerUser` | | User ID to login to the container registry<sup id="dockerUser">[4](#f4)</sup> | |
| `gpgVendorKeyName` | | Name of vendor signing key | |
| `gpgVendorKeyPassphrase` | | Passphrase for vendor signing key | |
| `gpgVendorPriFile` | | Path to file containing private key value of vendor signing key | |
| `gpgVendorPubFile` | | Path to file containing public key value of vendor signing key | |
| `hpvsName` | Y | Name to give your new HPVS instance | `fhetoolkit-s390x-sample`|
| `location` | Y | HPVS instance deployment location | `dal13` |
| `namespace` | | Name of container registry namespace <sup id="namespace">[5](#f5)</sup> | |
| `registrationFile` | Y | Name of registration file to be created for your new HPVS instance | `hpvs-fhe-registration.txt`
| `registryURL` | | URL of the container registry | |
| `repoPassphrase` | | Passphrase for repository signing key | |
| `resource_group` | Y | ID of IBM Cloud Resource Group to use for your deployment | Default Resource Group for your IBM Cloud account|
| `resource_plan_id` | Y | ID of the IBM Cloud Resource Plan to use for your deployment | HPVS "lite-s" plan (ID=`bb0005a1-ec13-4ee4-86f4-0c3b15a357d5`) |
| `rootPassphrase` | | Passphrase for root signing key | |

##### Notes
<b><sup id="f1">1</sup></b> If using DockerHub as your container repository, keep this field empty.  For IBM Cloud Container Registry, use `https://xx.icr.io:4443` where `xx` is the 2 digit country code (eg: `us`, `jp`, etc.) [↩](#DCTServer)<br>
<b><sup id="f2">2</sup></b> When `delegationkey` is `false`, then `delegationPriFile`, `delegationPubFile`, `delegationkeyName`, and `delegationPassphrase` are treated as having been unspecified.  When `delegationkey` is `true`, then `delegationPubFile`, `delegationkeyName`, and `delegationPassphrase` are required parameters.  For more information on delegation keys, please [see below](#delegation-keys). [↩](#delegationkey)<br>
<b><sup id="f3">3</sup></b> This field should not be specified if the delegation private key is already in the local Docker trust store [↩](#delegationPriFile)<br>
<b><sup id="f4">4</sup></b> If using IBM Cloud with the platform API key, `dockerUser` must be `iamapikey` and `dockerPW` must be your API key (IBM Cloud calls it an "IAM key"). You can get an IAM key by following the instructions (more or less) here: https://tinyurl.com/y529hkwf [↩](#dockerPW),<br>
<b><sup id="f5">5</sup></b> If using DockerHub, you can default to the name of your DockerHub account.  If using IBM Cloud Container Registry, a new namespace must first be created via the [web UI](https://cloud.ibm.com/registry/namespaces), or using the IBM Cloud CLI command: `ibmcloud cr namespace-add YOURNAMESPACE`. [↩](#namespace)<br>

#### The Keys
To sign and deploy an image to HPVS, we need three to four sets of keys:
1. Root key; root of content trust for an image tag
2. Repository key (aka "target key"); allows you to sign image tags and manage delegations (including delegated keys or permitted delegation paths)
3. Encryption key for the HPVS registration definition file
4. Vendor key, required for signing the HPVS registration definition file
5. Delegation key (optional); allows you to delegate signing image tags to other publishers without having to share your repository key
 
Before you can run `DeployToHPVS.sh`, you must tell it:
1. The passphrase to use for the root key
2. The passphrase to use for the repository key
3. The name and location of the GPG vendor key (both public and private)
4. The name and location of the delegation key (optional)

##### Automatic key generation
If `DeployToHPVS.sh` called with the `-c` option, the interactive wizard will walk you through the process of key generation.

The root and repository keys are generated automatically by `docker trust`, which is invoked by `DeployToHPVS.sh`.  The HPVS registration encryption key is hardcoded into `DeployToHPVS.sh`.  The other keys (the vendor and delegation keys) must be generated ahead of time.

##### Vendor key

To generate a vendor keypair, create a GPG batch file with the following contents:
```
%echo Generating registration definition key
Key-Type: RSA
Key-Length: 4096
Subkey-Type: RSA
Subkey-Length: 4096
Name-Real: fhe_user
Expire-Date: 0
Passphrase: <passphrase>
%echo done
```
Then run the following to generate a new keypair as the `fhe_user` identity:
```
gpg -a --batch --generate-key <batchfile>
```

##### Delegation keys
Delegations in Docker Content Trust (DCT) allow you to control who can and cannot sign an image tag. A delegation will have a pair of private and public delegation keys. A delegation could contain multiple pairs of keys and contributors in order to allow multiple users to be part of a delegation, and to support key rotation.

If `delegationkey` is `false` or commented-out in your configuration file, then the repository key will be used to sign your image. Thereafter, any changes to your image can only be made using the repository key.

Delegation keys can be generated using `docker trust`:
```
docker trust key generate <yourkeyname>
```