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


# The purpose of this script is to create documentation local on the host where FHE toolkit
# source code and IDe configuration can stored for quick reference.

# Initialize the container image name and id as empty for further consistency checks
ToolkitImageName=""
ToolkitImageId=""

# Formatting helpers
bold=$(tput bold)
normal=$(tput sgr0)

#
BASEDIR="$PWD"/$(dirname $0)
SCRIPTNAME=$(basename $0)

print_usage(){
  cat <<EOF

${bold}Usage: $scriptname [options] CONTAINER_IMG${normal}

${bold}CONTAINER_IMG${normal}     Selects the name of toolkit container image to render the documentation from.
                  This should be the IBMCOM pre-built toolkit from Docker Hub 
                  or the locally built toolkit tagged container name. This script
                  expects this container image name to exist in the local docker image catalog.
                  Supported container OS are:
                  x86_64/amd64: {ubuntu, fedora, centos, alpine}
                  s390x:        {ubuntu}

${bold}Options:${normal}
-h                Displays this help information.

${bold}Example:${normal}

To render documentation from a local toolkit build:
WriteMLHElibAPIDocs.sh local/fhe-toolkit-ubuntu

To render documentation from a fetched toolkit build:
WriteMLHElibAPIDocs.sh ibmcom/fhe-toolkit-fedora


EOF
}


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

# The default location on this host where the project docs will live
DOCS_BASE_PATH="$PWD"/Documentation/docs


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

DOCS_BASE_PATH=$2
DOCS_HELIB_PATH="$DOCS_BASE_PATH/ml-helib"

echo "Creating Documentation and adding it Locally: ${DOCS_BASE_PATH}"
  if ! mkdir -p "$DOCS_HELIB_PATH/"
  then
    echo " "
    echo "FATAL:  Fail to create $DOCS_HELIB_PATH ."
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
  else # Copy the generated ML-HElib Documentation to the local docs space ...
    if ! docker cp $ToolkitImageId:/opt/IBM/FHE-distro/ML-HElib/documentation/ML-HElib/html/. "$DOCS_HELIB_PATH/"
    then
      echo " "
      echo "FATAL:  Failed copying Documentation from container image to local directory on host"
      echo "        Please check for access permissions to ${DOCS_HELIB_PATH} and any error"
      echo "        messages above"
      echo " "
      exit -10
    else # Remove temporary file access to container image
      if ! docker rm -v $ToolkitImageId
      then
        echo " "
        echo "FATAL:  Failure removing temporary file access to container image."
        echo "        Please check for any error messages above"
        echo " "
        exit -11
      else
        echo " "
        echo "Documentation ready for use"
        echo " "
      fi
    fi
  fi
