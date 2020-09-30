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
# SOFTWARE


source ConfigConstants.sh

# We default to persistent storage because this is a code development toolkit and developers likely will make code changes and expect that to live on...
SANDBOX_MODE=0

# We default to running a container pulled from Docker Hub
CONTAINER_MODE=""

# Set default port for docker to use
PORT=8443

# The default location on this host where a project directory of FHE toolkit source code will be persisted
DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH="$PWD"/FHE-Toolkit-Workspace
zCX=$(docker system info | grep platform)
if [ $zCX  ] && [ $zCX == "platform=zOS" ]; then
  DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH=AZD_SHARED_VOLUME
fi

# Formatting helpers
bold=$(tput bold)
normal=$(tput sgr0)

#
BASEDIR="$PWD"/$(dirname $0)
SCRIPTNAME=$(basename $0)

print_usage(){
  cat <<EOF

${bold}Usage: $scriptname <-p|-l> [options] CONTAINER_OS${normal}

${bold}CONTAINER_OS${normal}   Selects the name of the operating system platform for
               the FHE Toolkit container you want to use.
               Available OS are:
               x86_64/amd64: {ubuntu, fedora, centos, alpine}
               s390x:        {ubuntu}

-p             Run the IBMCOM pre-built toolkit from Docker Hub

-l             Run the locally built toolkit

${bold}Options:${normal}
-v <directory> Specify a nonstandard directory for the persisted workspace.
               By default the IDE workspace will be persisted in
               $DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH

-s             Run the toolkit in stateless mode without a persisted workspace.

-h             Displays this help information.


EOF

}



#echo "$@"

# The number of parameters passed to this script
NPARAM=$#

while getopts ":hpslv:" opt; do
  case ${opt} in
    h ) # Usage
      print_usage
      exit 0
      ;;
    v ) # User wants a persistent volume. No sandbox. 
      FHE_WORKSPACE_PATH=$OPTARG
      SANDBOX_MODE="0"
      ;;
    s ) # User wants an ephemeral sandbox. No peristent volume.
      SANDBOX_MODE="1"
      ;;
    l ) # Run locally built container
      [ -n "$CONTAINER_MODE" ] \
      && { echo "Fatal: Options -l and -p are mutually exclusive" ; print_usage; exit -1;} \
      || CONTAINER_MODE=local
      ;;
    p ) # Run ibmcom pre-built container
      [ -n "$CONTAINER_MODE" ] \
      && { echo "Fatal: Options -l and -p are mutually exclusive" ; print_usage; exit -2; } \
      || CONTAINER_MODE=ibmcom
      ;;
    \? ) # Usage
      print_usage
      exit 0
      ;;
    : ) # Invalid option. Print usage
      echo "Invalid option: -$OPTARG requires an argument" 1>&2
      print_usage
      exit -3
      ;;
  esac
done

shift $((OPTIND -1))
#echo "$@"

OPTINDNUMBER=$((OPTIND-1))

platform=$1

if [[ $((NPARAM - OPTINDNUMBER)) -ne 1 ]]; then
  echo "Fatal: Options and parameters mismatch or missing parameters. Please check order and number of parameters."
  print_usage
  exit -4
fi



# Determine which container distributuon to launch
if ! [ "$platform" ]
then
  echo "Missing OS platform name - Please specify a supported platform"
  print_usage
  exit -5
elif [[ "$platform" = "fedora" ]]
then
    platform="fedora"
    tag="32"
elif [[ "$platform" = "centos" ]]
then
    platform="centos"
    tag="8"
elif [[ "$platform" = "ubuntu" ]]
then
    platform="ubuntu"
    tag="20.04"
elif [[ "$platform" = "alpine" ]]
then
    platform="alpine"
    tag="3.12"    
else
    echo "Invalid option: $platform - Please specify a supported platform"
    print_usage
    exit -6
fi

# We need to use absolute paths for the -v volume option, and users like to use relative paths, so make it work for them...
function abs_path {
  local dir=`dirname "$1"`
  local file=`basename "$1"`
  pushd "$dir" &>/dev/null || return $? # On error, return error code
  echo "`pwd -P`/$file" # output full, link-resolved path with filename
  popd &> /dev/null
}


# Here we handle user options with respect to ephemeral or persistent code storage within the toolkit.
# Define an empty string or a valid docker volume option to contain the users intent
DOCKER_PERSISTENT_VOLUME_ARGS=""
# If we are running persistent mode, respect the users invocation...
if [ "$SANDBOX_MODE" == "0" ]; then 
  if [ ! "${FHE_WORKSPACE_PATH}" ]; then
    FHE_WORKSPACE_PATH="${DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH}"
    echo "INFO:    Using system default persistent storage path..."
  else
    echo "INFO:    Using user-specified persistent storage path..."
  fi

  # Convert to absolute path to protect against relative paths unless on zCX
  if [ ! $zCX ]; then
  FHE_WORKSPACE_PATH=$(abs_path "$FHE_WORKSPACE_PATH")
  fi
  DOCKER_PERSISTENT_VOLUME_ARGS="-v ${FHE_WORKSPACE_PATH}:/opt/IBM/FHE-Workspace "
  echo "INFO:    Persistent data storage: \"${FHE_WORKSPACE_PATH}"\"

  # Now we have some kind of Persistent path to use, we should check that it is sane (writeable)
  # Note we check against the unescaped path version
  if [ ! -w "$FHE_WORKSPACE_PATH" ] && [ ! $zCX ]; then
    echo "FATAL:   The requested persistent path \"$FHE_WORKSPACE_PATH\" does not exist and cannot be used for persistent storage of the fhe toolkit runtime."
    echo "         Please check that this directory exists and contains a valid project workspace. One can use the provided PersistData.sh script to set up a location."
    exit -7
  fi

  # When using the default path, make sure the cmake configuraion is cleaned each time
  if [ "$FHE_WORKSPACE_PATH" == "$DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH" ]; then
    echo "INFO:    CMake: Deleting cached built settings and reconfigure"
    CMAKE_CACHE=$DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH/build/CMakeCache.txt
    if [[ -f "$CMAKE_CACHE" ]]; then
      rm $CMAKE_CACHE
    fi
    CMAKE_FILES=$DEFAULT_FHE_PERSISTENT_WORKSPACE_PATH/build/CMakeFiles
    if [[ -d "$CMAKE_FILES" ]]; then
      rm -r $CMAKE_FILES
    fi
  fi

  pattern=" |'|\"|!"
  if [[ $FHE_WORKSPACE_PATH =~ $pattern ]]; then
    echo "FATAL:   The requested persistent path \"$FHE_WORKSPACE_PATH\" must not contain spaces or some special characters."
    echo "         Please check that this directory path does not contain spaces or any special characters or non printable characters"
    exit -8
  fi

# If we are in ephemeral mode we can leave the docker volume arg string entirely blank to be ignored at docker run time
else
  :
  echo "INFO:    User-specified sandbox mode with no persistent data storage"
  # The container will use the included file system that is ephemeral for the example in the well known path /opt/IBM/HE-Workspace/
fi

# Check for LOCAL vs Docker Hub build choice
if [ ${CONTAINER_MODE} == "local" ]; then
  FHEkit_image_name=local/fhe-toolkit-${platform}
  FHEkit_container_name=local-fhe-toolkit-${platform}
elif [ ${CONTAINER_MODE} == "ibmcom" ]; then
  FHEkit_image_name=ibmcom/fhe-toolkit-${platform}
  FHEkit_container_name=fhe-toolkit-${platform}
else
  echo "FATAL: Container mode $CONTAINER_MODE is invalid"
  print_usage
  exit -9
fi

#exit 0

CID=$(docker ps -q -f status=running -f name=fhe)
if [ ! "${CID}" ]; then
  # Container is not running...

  FHE_WORKSPACE_PATH=$( echo "${FHE_WORKSPACE_PATH}" | sed 's/ /\\ /g' )
  
  cmd="docker run -d --name $FHEkit_container_name  $DOCKER_PERSISTENT_VOLUME_ARGS -p $PORT:8443 $FHEkit_image_name"

  echo "INFO:    Launching FHE tookit: "
  echo ""
  echo ""
  echo "         $cmd"
  echo ""
  echo ""
  $cmd
  
  DOCKER_RET_CODE=$?
  if [ $DOCKER_RET_CODE -ne "0" ]; then
      echo ""
      echo ""
      echo "****************************************************************************"
      echo "FATAL:    Docker container launch failed. Aborting."
      echo "          Make sure you run the FetchDockerImage.sh script for pre-packaged"
      echo "          images or the BuildDockerIMage.sh script for locally built images."
      echo "****************************************************************************"
      echo ""
      echo ""
      exit -10
  fi 
  # If we made it here print the info about the new running container...
  echo ""
  docker ps -a --filter name=fhe-toolkit-$platform
  echo "" 
  echo "FHE Development is open for business: https://127.0.0.1:$PORT/"
else
  echo ""
  echo ""
  echo "*****************************************************************************************************"
  echo "WARNING: An existing instance already running. Only a single FHE toolkit instance may run presently. "
  echo "         The instance must be stopped before a new toolkit instance can be launched:" 
  echo "*****************************************************************************************************"
  echo "" 
  echo "" 
  docker ps -a -f name=fhe-
  echo "" 
fi
unset CID


