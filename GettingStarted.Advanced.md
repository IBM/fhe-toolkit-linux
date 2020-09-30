----
# Advanced Guide to Building and Running the IBM FHE Toolkit for Linux

For advanced users, this guide will take you through the steps required
to build the IBM FHE Toolkit docker image from the Dockerfiles provided 
in this git repository.

## Unsupported Configurations

This project strives to support as many Docker capable host platforms as possible, however due to recent changes in Fedora 31 and 32, Docker has been reported not to work out of the box. Therefore, we can't support Fedora 31+ as a host for the toolkit. If you are interested in testing the toolkit as a Fedora container, you can use the pre-built container from Docker hub or build the Fedora toolkit container yourself on MacOS 10.13+ , Ubuntu 20.04, CentOS 8 or Alpine 3.12. There is no plan to add host support by the development team at this time, but we would happily accept patches from the community to add host support if possible without complicating builds on the majority of our supported platforms.

## Prerequisites

Before you can run this toolkit you must clone this repo. This tutorial 
assumes you have a working internet connection, a functioning `git` 
installation, a working `Docker` installation in your system and the 
necessary user privileges to run `docker` commands.

## Step 1: Cloning the IBM FHE Toolkit Repository

First, create a folder of your choice to hold the toolkit distribution.

Open a terminal window and `cd` into the folder you created, and issue 
the following command to clone this git repo:

```
git clone https://github.com/ibm/fhe-toolkit-linux
```

Once the source code has been cloned to your local development system,
you are ready to get started building the IBM FHE Toolkit docker image.

## Step 2: Building the Docker Images

To build the Docker image for one of the available platforms, first go to the folder where you cloned the 
toolkit and `cd` into the toolkit project folder. 

```
cd fhe-toolkit-linux
```

The build is initiated by invoking the `BuildDockerImage.sh` script with `-p <platform>` flag to select one of the supported platforms (`centos`, `ubuntu`, `fedora` or `alpine`). The remainder of these instructions will use CentOS as the example platform.

```
./BuildDockerImage.sh centos
```
Upon completion of the `BuildDockerImage.sh` script, you can verify the images were built successfully by issuing the following command:

```
docker images
```

> **_NOTE:_** Depending on your system, this build step may take a few minutes to run.

## Step 3: Running the Toolkit

Once the build script has completed, invoke the `RunToolkit.sh` script with the `-l <platform>` flag. The -l flag specifies the locally built and tagged container. This will start the FHE Toolkit container with web-accessible IDE running as a daemon, ready to be accessed with a web browser.

```
./RunToolkit.sh -l centos
```

Now that the toolkit container is up and running, you can follow the instructions in [GettingStarted.md -  ***_Step 4: Accessing the Toolkit_***](GettingStarted.md#step-4-accessing-the-toolkit) to access it and run the example program. 


## Optional Advanced Setup: Persistent Data Modes

By default, the toolkit uses a volume to persist data to the local directory `FHE-Toolkit-Workspace`. To run the toolkit in Sandbox mode as an ephemeral container, run with the `-s` flag. Substitute the "-l" flag for the "-p" flag as appropriate for your needs. 

```
./RunToolkit.sh -p -s centos
```

To specify a different location for the toolkit volume to persist data locally, use the `-v <directory>` flag when running the toolkit. Make sure the directory exist before running this. Substitute the "-l" flag for the "-p" flag as appropriate for your needs. 

```
./RunToolkit.sh -p -v my-workspace centos
```

## Optional Advanced Setup: Configure CMake Source Directory
Once you have access the toolkit, you'll see that the CMake source directory is set in `.vscode/settings.json` within the toolkit. You can update this file if you would like to configure a different source directory for CMake to use, for example if you want to run another example program.

![Step four image](/Documentation/Images/Advanced_1.png?raw=true "Configure IDE")
