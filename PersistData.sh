#!/bin/bash

# MIT License
#
# Copyright (c) 2020 International Business Machines
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# The purpose of this script is to setup a persistent location where FHE toolkit
# source code and IDe configuration can persist across container instantiations.
# It is not needed if sandbox mode is used (i.e., when no persistence is desired)
# 
# Most users will want to persist their source code changes, and this script sets
# up a location within the host file system to persist some state.


echo ".............................................................."
# Print how we were invoked...
echo "Invoked: $(printf %q "$BASH_SOURCE")$((($#)) && printf ' %q' "$@")"
echo ".............................................................."


# Initialize the container image name and id as empty for further consistency checks
ToolkitImageName=""
ToolkitImageId=""

# The default location on this host where a project directory of FHE toolkit source
# code will be persisted
PERSISTENT_FHE_WORKSPACE_PATH="$PWD"/FHE-Toolkit-Workspace
# Check if we need to us zCX specific persistent workspace path
zCX=$(docker system info | grep platform)
if [ $zCX  ] && [ $zCX == "platform=zOS" ]; then
  PERSISTENT_FHE_WORKSPACE_PATH=/media/azd_shared_volume
fi

# Formatting helpers
bold=$(tput bold)
normal=$(tput sgr0)

#
BASEDIR="$PWD"/$(dirname $0)
SCRIPTNAME=$(basename $0)

print_usage(){
  cat <<EOF

${bold}Usage: $scriptname [options] CONTAINER_IMG${normal}

${bold}CONTAINER_IMG${normal}     Selects the name of toolkit container image to persist data from.
                  This should be the IBMCOM pre-built toolkit from Docker Hub 
                  or the locally built toolkit tagged container name. This script
                  exects this container image name to exist in the local docker image catalog.
                  Supported container OS are:
                  x86_64/amd64: {ubuntu, fedora, centos, alpine}
                  s390x:        {ubuntu}

${bold}Options:${normal}
-h                Displays this help information.

${bold}Example:${normal}

To persist a local toolkit build:
PersistData.sh local/fhe-toolkit-ubuntu

To persist a fetched toolkit build:
PersistData.sh ibmcom/fhe-toolkit-fedora


EOF
}

#Check for supported host architecture
ARCH=`uname -m`

if [[ "$ARCH" == "x86_64" ]] || [[ "$ARCH" == "amd64" ]]; then
  #echo "Determined AMD64/x86_64 Architecture"
  architecture="AMD64/x86_64"
  ARCH="amd64"
elif [[ $ARCH == "s390x" ]]; then
  # echo "Determined s390x Architecture"
  architecture="s390x"
else
  echo " "
  echo " FATAL: Aborting. $ARCH is not a suppported platform for hosting or building the FHE Toolkit."
  echo " "
  exit -1
fi

# The number of parameters passed to this script
NPARAM=$#

while getopts ":h" opt; do
  case ${opt} in
    h ) # Usage
      print_usage
      exit 0
      ;;
    \? ) # Usage
      print_usage
      exit 0
      ;;
    : ) # Invalid option. Print usage
      echo "Invalid option: -$OPTARG requires an argument" 1>&2
      print_usage
      exit -1
      ;;
  esac
done

# Check we have the right number of mandatory parameters
shift $((OPTIND -1))
OPTINDNUMBER=$((OPTIND-1))

if [[ $((NPARAM - OPTINDNUMBER)) -ne 1 ]]; then
  echo " "
  echo "FATAL: Options and parameters mismatch or missing parameters. Please check order and number of parameters."
  echo " "
  print_usage
  exit -2
fi

# Since we have one parameter, let's initialize the container image
ToolkitImageName=$1
if ! [ $ToolkitImageName ]
then
  echo " "
  echo " Mandatory parameter ${bold}CONTAINER_IMG${normal} is empty."
  echo " "
  print_usage
  exit -3
fi

IFS='/-' read -a ToolkitImageNameTokens <<< "$ToolkitImageName"
tokenSize=${#ToolkitImageNameTokens[@]}
token0=${ToolkitImageNameTokens[0]} # <local|ibmcom>
token1=${ToolkitImageNameTokens[1]} # fhe
token2=${ToolkitImageNameTokens[2]} # toolkit
platform=${ToolkitImageNameTokens[3]} # {ubuntu, fedora, centos}

if ! [ $tokenSize -eq 4 ]
then
  echo " "
  echo "FATAL: Mandatory parameter ${bold}CONTAINER_IMG${normal} is malformed."
  echo " "
  print_usage
  exit -4
fi

if [ $ARCH == "s390x" ]
then
  if ! [ "$platform" = "ubuntu" ] \
  && ! [ "$platform" = "fedora" ] \
  && ! [ "$platform" = "alpine" ] 
  then
    echo " "
    echo " Invalid platform: $ToolkitImageName ($platform on $ARCH) is not supported."
    echo " Please specify a support platform"
    echo " "
    print_usage
    exit -5
  fi
elif [ $ARCH == "amd64" ]
then
  if ! [ "$platform" = "fedora" ] \
  && ! [ "$platform" = "centos" ] \
  && ! [ "$platform" = "ubuntu" ] \
  && ! [ "$platform" = "alpine" ]
  then
    echo " "
    echo " Invalid platform: $ToolkitImageName ($platform on $ARCH) is not supported."
    echo " Please specify a support platform"
    echo " "
    print_usage
    exit -6
  fi
else
  echo " "
  echo " FATAL: Aborting. $ARCH is not a suppported platform for hosting or building the FHE Toolkit."
  echo " "
  exit -7
fi

# Now we check whether the persistent location already exists. If if it does, we will exit and will try to use it
# If we're on zCX, the persistent location will already exist.
if [ ! "$(ls -A ${PERSISTENT_FHE_WORKSPACE_PATH})" ] || [ $zCX ]
then
  echo "Creating FHE-Workspace and adding FHE examples: ${PERSISTENT_FHE_WORKSPACE_PATH}"
  if ! mkdir -p "$PERSISTENT_FHE_WORKSPACE_PATH/"
  then
    echo " "
    echo "FATAL:  Fail to create $PERSISTENT_FHE_WORKSPACE_PATH ."
    echo "        Please check you have the necessary permission to create directories"
    echo " "
    exit -8
  fi
  # Set up access to a container instance file system without running the container...
  if ! ToolkitImageId=$(docker create $ToolkitImageName)
  then
    echo " "
    echo "FATAL:  Could not find container $ToolkitImageName in your system or in Docker HUB,"
    echo "        or Docker utility is not installed. Please check that you have a working Docker"
    echo "        installation in your system and the necessary user privileges to run docker commands."
    echo " "
    echo "        Also check, using the ${bold}docker images${normal} command that you have the docker"
    echo "        image in your system"
    echo " "
    exit -9
  else # Copy the upstream HElib example and IDE configutation to the persistent workspace ...
    if ! docker cp $ToolkitImageId:/opt/IBM/FHE-Workspace/. "$PERSISTENT_FHE_WORKSPACE_PATH/"
    then
      echo " "
      echo "FATAL:  Failed copying FHE Workspace from container image to locally persited workspace on host"
      echo "        Please check for access permissions to ${PERSISTENT_FHE_WORKSPACE_PATH} and any error"
      echo "        messages above"
      echo " "
      exit -10
    else # Remove temporary file access to container image
      if ! docker rm -v $ToolkitImageId
      then
        echo " "
        echo "FATAL:  Falure removing temporary file access to container image."
        echo "        Please check for any error messages above"
        echo " "
        exit -11
      else
        echo " "
        echo "Persistent FHE-Workspace ready for use"
        echo " "
      fi
    fi
  fi
else
  echo "Using existing FHE-Workspace: $PERSISTENT_FHE_WORKSPACE_PATH"
  exit 1
fi

if [ $? -ne 0 ]
then
        echo "Failed to setup workspace"
        exit -1
fi
