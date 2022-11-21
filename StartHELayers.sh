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


# This script is intended for those who want to try a pre-built docker container for their toolkit distribution of choice.
# see the script called BuildDockerContainer if you want to build or modify your own instance. 

source ConfigConstants.sh

bold=$(tput bold)
normal=$(tput sgr0)

BASEDIR="$PWD"/$(dirname $0)
SCRIPTNAME=$(basename $0)


print_usage(){
  cat <<EOF

  ${bold}Usage: $SCRIPTNAME [options] LAB_TYPE${normal}

  ${bold}LAB_TYPE${normal}   Selects the type of lab to
                 download the HELayers container from IBM Container Registry.
                 Available lab types are:
                 x86_64/amd64: {cpp, python}
                 s390x:        {cpp, python}

  ${bold}OPTIONS:${normal}
  -h             Displays this help information.

  ${bold}Examples:${normal}
  Obtain the HELayers container with the python lab.
  $SCRIPTNAME python

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
  echo " FATAL: Aborting. HELayers is not supported on the $ARCH platform."
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

labtype=$1
if [ $ARCH == "s390x" ]
then
  if [[ "$labtype" = "python" ]]
  then
      imagesuffix="pylab-s390x"
  elif [[ "$labtype" = "cpp" ]]
  then
      imagesuffix="lab-s390x"
  else
    echo "INFO: Lab type: $labtype on $ARCH is not supported on this release."
    echo "      Please choose a supported lab type for your trials by specifying"
    echo "      the command: ${bold}$SCRIPTNAME <lab | python>${normal}"
    echo " "
    exit -1
  fi
else
  if [ $ARCH == "amd64" ]; then
    if [[ "$labtype" = "python" ]]
      then
      imagesuffix="pylab"
      labport="8888"
    elif [[ "$labtype" = "cpp" ]]
    then
      imagesuffix="lab"
      labport="8443"
    else
      echo " "
      echo "  Invalid lab type: $labtype Please specify a valid lab type"
      echo " "
      print_usage
      exit -5
    fi
  fi
fi

image="icr.io/helayers/helayers-$imagesuffix:latest"

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
echo " Fetching HELayers (latest) for Linux"
echo " Architecture: $architecture "
echo " Lab: $labtype "
echo ""
echo "==============================================================="

docker pull "$image"
if [ $? -ne 0 ]; then
  echo " "
  echo " FATAL: Aborting. There was an issue pulling the image $image"
  echo " "
  exit -1
fi

docker run -p $labport:$labport -d --rm --name helayers-$imagesuffix "$image"
if [ $? -ne 0 ]; then
  echo " "
  echo " FATAL: Aborting. There was an issue running the image $image"
  echo " "
  exit -1
fi

if [[ "$labtype" = "python" ]]
then
  echo " "
  echo " Container started. Navigate to: http://127.0.0.1:8888/lab/?token=demo-experience-with-fhe-and-python  "
  echo " "
elif [[ "$labtype" = "cpp" ]]
then
  echo " "
  echo " Container started. Navigate to: http://127.0.0.1:8443/ "
  echo " "
fi

exit 0
