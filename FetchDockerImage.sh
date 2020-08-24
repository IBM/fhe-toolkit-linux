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


# This script is intended for those who want to try a pre-built docker container for their toolit distribution of choice.
# see the script called BuildDockerContainer if you want to build or modify your own instance. 

HElib_version='v1.0.2'

bold=$(tput bold)
normal=$(tput sgr0)

BASEDIR="$PWD"/$(dirname $0)
SCRIPTNAME=$(basename $0)


print_usage(){
  cat <<EOF

  ${bold}Usage: $SCRIPTNAME [options] CONTAINER_OS${normal}

  ${bold}CONTAINER_OS${normal}   Selects the name of the operating system platform to
                 download the FHE Toolkit container from Docker Hub.
                 Available OS are:
                 x86_64/amd64: {ubuntu, fedora, centos, alpine}
                 s390x:        {ubuntu, fedora, alpine}

  ${bold}OPTIONS:${normal}
  -h             Displays this help information.

  ${bold}Examples:${normal}
  Obtain the toolkit container based on Fedora.
  $SCRIPTNAME fedora

  Obtain the toolkit container based on Ubuntu.
  $SCRIPTNAME ubuntu

  Obtain the toolkit container based on CentOS.
  $SCRIPTNAME centos

  Obtain the toolkit container based on Alpine.
  $SCRIPTNAME alpine

  Show this help message.
  $SCRIPTNAME -h


EOF
}

# The number of parameters passed to this script
NPARAM=$#

# First we determine which architecture we are running on... AMD64 or s390x
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
  echo " FATAL: Aborting. The FHE Toolkit is not supported on the $ARCH platform."
  echo " "
  exit -1
fi

#echo "$@"
NPARAM=$#

while getopts ":h" opt; do
  case ${opt} in
    h ) # Usage
      print_usage
      exit 0
      ;;
   \? ) # Usage
     echo "Fatal: Invalid options"
     print_usage
     exit -2
     ;;
   : ) # Invalid option. Print usage
     echo "Fatal: Invalid option: $OPTARG requires an argument" 1>&2
     print_usage
     exit -3
  esac
done

shift $((OPTIND -1))
#echo "$@"

OPTINDNUMBER=$((OPTIND-1))

if [[ $((NPARAM - OPTINDNUMBER)) -ne 1 ]]; then
  echo "Fatal: Options and parameters mismatch or missing parameters. Please check order and number of parameters."
  print_usage
  exit -4
fi

platform=$1
if [ $ARCH == "s390x" ]
then
  if [[ "$platform" = "ubuntu" ]]
  then
    build="UBUNTU"
    platform="ubuntu"
    release="20.04"
  elif [[ "$platform" = "fedora" ]]
  then
      build="FEDORA"
      platform="fedora"
      release="32"
  elif [[ "$platform" = "alpine" ]]
  then
      build="ALPINE"
      platform="alpine"
      release="3.12"
  else
    echo "INFO: Platform: $platform on $ARCH is not supported on this release."
    echo "      Please choose a supported platform for your trials by specifying"
    echo "      the command: ${bold}$SCRIPTNAME <ubuntu | fedora>${normal}"
    echo " "
    exit -1
  fi
else
  if [ $ARCH == "amd64" ]; then
    if [[ "$platform" = "fedora" ]]
      then
      build="FEDORA"
      platform="fedora"
      release="32"
    elif [[ "$platform" = "centos" ]]
    then
      build="CENTOS"
      platform="centos"
      release="8"
    elif [[ "$platform" = "ubuntu" ]]
    then
      build="UBUNTU"
      platform="ubuntu"
      release="20.04"
    elif [[ "$platform" = "alpine" ]]
    then
      build="ALPINE"
      platform="alpine"
      release="3.12"  
    else
      echo " "
      echo "  Invalid platform: $platform Please specify a support platform"
      echo " "
      print_usage
      exit -5
    fi
  fi
fi

platform_tag=$platform-$ARCH

################################################################
# The script has the following arguments:
#    $(1): list of tags of the image
if [ $# -gt 0 ]; then
        tags=("$@")
fi

# Looks for an environment var named DOCKER_BUILD_FLAGS. If not
# defined, uses the default flag value '--no-cache' for the docker build.
# Ex.: To enable docker caching, export DOCKER_BUILD_FLAGS="" (empty value)
#      To then disable docker caching again, unset DOCKER_BUILD_FLAGS
#flags=${DOCKER_BUILD_FLAGS-"--no-cache"}
flags=${DOCKER_BUILD_FLAGS-""}

echo "==============================================================="
echo ""
echo " Phase 1: Downloading dependencies locally"
echo ""
echo " Fetching FHE Toolkit $HElib_version for Linux"
echo " Architecture: $architecture "
echo " Platform: $platform_tag "
echo ""
echo "==============================================================="

docker pull ibmcom/fhe-toolkit-$platform_tag
docker tag ibmcom/fhe-toolkit-$platform_tag ibmcom/fhe-toolkit-$platform
if [ $? -ne 0 ]; then
  echo " "
  echo " FATAL: Aborting. There was an issue pulling the image ibmcom/fhe-toolkit-$platform "
  echo " "
  exit -1
fi

echo "==============================================================="
echo ""
echo " Phase 2: Setup Toolkit Persistent Working Directory"
echo ""
echo "==============================================================="
cd $BASEDIR
./PersistData.sh ibmcom/fhe-toolkit-$platform

exit 0
