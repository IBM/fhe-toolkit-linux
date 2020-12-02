----
# Getting Started with the IBM FHE Toolkit for Linux

This document is a step-by-step guide to installing the IBM FHE Toolkit 
and running the examples. You'll be able to access the FHE Toolkit through 
a web browser running in a Docker container.

## Prerequisites

Before you can run this toolkit you must clone this repo. This tutorial 
assumes you have a working internet connection, a functioning `git` 
installation and a working, and recent `Docker` installation in your system and the 
necessary user privileges to run `docker` commands. At present Docker version 19 or higher is required. Older versions of Docker are known not to work properly with our system, and there are no plans by the maintainers to support older Docker versions. 

A working internet connection is required to build the toolkit as some dependencies are fetched from external sources at build time. Similarly a working internet connection is required if a user opts to use a pre-built image from DockerHub.

## Step 1: Cloning the IBM FHE Toolkit Repository
First, from a terminal
window, issue the following command to clone this git repo:

```
git clone https://github.com/ibm/fhe-toolkit-linux
```

## Step 2: Fetching the Toolkit Docker Images

To fetch a Toolkit Docker image, first go to the folder where you cloned
the toolkit and `cd` into the toolkit project folder. 

```
cd fhe-toolkit-linux
```

Fetch the Toolkit Docker images by invoking the `FetchDockerImage.sh` 
script followed by `<platform>` to select one of the supported platforms 
(`centos`, `ubuntu`, `fedora` or `alpine`). The remaining of these instructions will use CentOS as the example platform.

```
./FetchDockerImage.sh centos
```

Check the image `ibmcom/fhe-toolkit-centos` was successfully downloaded 
from Docker Hub.

```
docker images
```

## Step 3: Running the Toolkit

Once the fetch script has completed, invoke the `RunToolkit.sh` script with 
the `-p <platform>` flag. This will start the FHE Toolkit container with 
the IDE running as a daemon, ready to be accessed with a web browser.

```
./RunToolkit.sh -p centos
```

## Step 4: Accessing the Toolkit

Open a web browser on your host machine (not the docker container instance) and browse to
<a target="_blank" href="https://127.0.0.1:8443/">https://127.0.0.1:8443/</a>. This will
connect you to the IDE running in the FHE toolkit Docker container.

Note that this connection is secured over https using a self-signed certificate. You'll therefore need to tell your browser to trust it each time you connect to a new instance of the toolkit. In Chrome, you can do this by clicking anywhere on the warning text and typing “thisisunsafe”.

![Step three image](/Documentation/Images/Step_4_Chrome_Warning.png?raw=true "IDE in a browser")

If you're using Safari, you'll need to click the `visit this website` link and enter your username and password as shown [here](Documentation/Images/Safari.png). For Firefox, click `Advanced...`, then click `Accept the Risk and Continue` as shown [here](Documentation/Images/Firefox.png).

## Step 5: Configuring the Toolkit

Once in the toolkit, you should automatically be prompted to select a kit for the FHE-Workspace to use. Select one of the kits from the dropdown. Configuration of the workspace will begin which you will be able to see in the Output Window. You'll also notice the kit you selected is now shown in the CMake Tools status bar at the bottom of the window.

If you are not automatically prompted to select a kit after a few seconds of loading the toolkit, try refreshing your browser. If you're still not prompted, check the CMake Tools status bar at the bottom of the window to see if a kit has already been selected. You'll need to select a kit each time you start a new instance of the Toolkit.

![Step five image](/Documentation/Images/Step_5A_Configure.png?raw=true "IDE in a browser")

![Step five image](/Documentation/Images/Step_5B_Configure.png?raw=true "IDE in a browser")


## Step 6: Building Your First HElib Example

Click "Build" in the CMake Tools status bar to build the selected target.

![Step five image](/Documentation/Images/Step_6.png?raw=true "Build")

## Step 7: Running the HElib Example (Point and Click)

When the build has finished, click "Launch" in the CMake Tools status bar to launch the selected target in the terminal window.

Each demonstration application is in a self contained directory in the examples folder in the IDE. Each demo application directory contains a *README.md* that explains how to run the demo and what you should
expect for results. For instance, the <a href="samples/BGV_country_db_lookup/README.md" target="_blank">BGV Country Database Lookup Example Documentation</a> contains the information to run a complete example of a privacy preserving search against an encrypted database. The database is a key value store prepopulated with the english names of countries and their capital cities from the continent of Europe. Selecting the country will perform a search of the matching capital. 

![Step six image](/Documentation/Images/Step_7A.png?raw=true "example running")

Follow the text in the demo specific documentation you have chosen. For example, if using the privacy preserving country database search, enter a European country as input to find out its capital city. In the image below, the example used is `Sweden`. 

![Step six image](/Documentation/Images/Step_7B.png?raw=true "example running")

Congratulations, you are running an example program using HElib!

Feel free to explore the template code in any of the examples to get familiar with HElib. Any changes made while working in the Toolkit will be persisted to your local file system in the `FHE-Toolkit-Workspace` directory.

## Optional Step 8: Stopping the Toolkit

Once you've finished using the toolkit instance, run the helper script
`./StopToolkit.sh` from the terminal in your host system to stop and remove all
toolkit instances.

```bash
./StopToolkit.sh
```

Any updates made to your `FHE-Toolkit-Workspace` directory remain in place so you can start where you left off next time you run the toolkit.

## Optional Step 9: Running the HElib Example (Integrated IDE Console)

As an alternative to using the "Launch" button in the CMake Tools status bar, you can use the integrated terminal.

> Should you need to configure a default shell different than
the one provide by the IDE on your Linux distribution, follow these instructions:
>
> - Press F1 in the IDE to open/go to the search.
> - Type `Select Default Shell`, to search for the option to set the terminal shell.
> - Select the `Terminal: Select Default Shell` entry.
> - Select `/usr/bin/bash` as your shell.
> - Close and reopen the integrated terminal window for the change of shell to
take effect.
>
>![Step eight image](/Documentation/Images/Step_9A.png?raw=true "example")
>![Step eight image](/Documentation/Images/Step_9B.png?raw=true "example")

In the terminal, change to the `build` directory and run the example program.

```bash
cd build
./BGV_country_db_lookup
```

![Step eight image](/Documentation/Images/Step_9C.png?raw=true "example")

