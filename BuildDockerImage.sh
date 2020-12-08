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

bold=$(tput bold)
normal=$(tput sgr0)

BASEDIR="$PWD"/$(dirname $0)
SCRIPTNAME=$(basename $0)

source ConfigConstants.sh

# z/OS Container Extensions platform CURL implementation does not work as expected. If zCX platform detected, add flag to adjust.
# adding -k option to curl commands on zCX else the string stays empty
CURL_FIX_ZCX=""

print_usage(){
  cat <<EOF

${bold}Usage: $SCRIPTNAME [options] CONTAINER_OS${normal}

${bold}CONTAINER_OS${normal}   Selects the name of the operating system platform to
               build the FHE Toolkit container.
               Available OS are:
               x86_64/amd64: {ubuntu, fedora, centos, alpine}
               s390x:        {ubuntu, fedora, alpine}

${bold}OPTIONS:${normal}
-h             Displays this help information.


EOF
}

get_NTL(){
  # We try to use curl to download the NTL
  echo "Checking for cached NTL download..."
  if [ ! -f ntl-$1.tar.gz ]; then 
      echo "INFO: Attempting to download NTL with the curl command..."
      if ! curl $CURL_FIX_ZCX -o ntl-$1.tar.gz https://www.shoup.net/ntl/ntl-$1.tar.gz
      then
        echo " FATAL: There was an issue downloading NTL-$1 from www.shoup.net."
      fi 
  fi
  # Now we untar whatever we fetched...
  echo "INFO: Uncompressing NTL..."
  if [ ! -d NTL ]; then
      mkdir NTL
  fi
  tar --no-same-owner -C NTL --strip-components=1 -xf ntl-$1.tar.gz
  if [ $? -ne 0 ]; then
    echo " "
    echo " FATAL: Aborting. There was an issue extracting files from ntl-$1.tar.gz"
    echo " "
    exit -1
  fi
  # Remove tar file
  #rm ntl-$1.tar.gz
}

get_HElib(){
  # We try to use curl to download the HElib distribution
  echo "Checking for cached HElib download..."
  if [ ! -f HElib-$1.tar.gz ]; then  
      echo "INFO: Attempting to download HElib $1 with the curl command..."
      if ! curl $CURL_FIX_ZCX -Lo HElib-$1.tar.gz https://github.com/homenc/HElib/archive/$1.tar.gz
      then
        echo " FATAL: There was an issue downloading HElib $1 from https://github.com/homenc/HElib ."
      fi
  fi
  # Now we untar whatever we fetched...
  echo "INFO: Uncompressing HElib..."
  if [ ! -d HElib ]; then
      mkdir HElib
  fi
  tar --no-same-owner -C HElib --strip-components=1 -xf HElib-$1.tar.gz
  if [ $? -ne 0 ]; then
    echo " "
    echo " FATAL: Aborting. There was an issue extracting files from HElib-$1.tar.gz"
    echo " "
    exit -1
  fi
  # Remove tar file
  #rm HElib-$1.tar.gz
}

get_Boost(){
  # We try to use curl to download Boost

  # If we already downloaded it, no need to repeateldy download the same specified version we depend on...
  echo "Checking for cached Boost download..."
  if [ ! -f boost-${Boost_version}.tar.gz ]; then
    echo "INFO: Attempting to download Boost with the curl command..." 
    if ! curl -fL $CURL_FIX_ZCX -o  boost-$1.tar.gz https://dl.bintray.com/boostorg/release/$1/source/boost_$2.tar.gz
    then
      echo " FATAL: There was an issue downloading boost_$2 from dl.bintray.com."
    fi

    # Now we untar whatever we fetched...
    echo "INFO: Uncompressing Boost..."
    tar --no-same-owner -xf boost-$1.tar.gz
    if [ $? -ne 0 ]; then
      echo " "
      echo " FATAL: Aborting. There was an issue extracting files from boost-$1.tar.gz"
      echo " "
      exit -1
    fi
    # Rename download and remove tar file
    mv boost_$2 boost
  fi
  #rm boost-$1.tar.gz
}



# First we determine which architecture we are running on... AMD64 or s390x
ARCH=`uname -m`

if [[ "$ARCH" == "x86_64" ]] || [[ "$ARCH" == "amd64" ]]; then
  #echo "Determined AMD64/x86_64 Architecture"
  architecture="AMD64/x86_64"
  ARCH="amd64"
elif [[ $ARCH == "s390x" ]]; then
  # echo "Determined s390x Architecture"
  architecture="s390x"
  ARCH="s390x"
  # Ony on s390x arch is a situation where we might be running in a zOS Container extensions Docker host. 
  # z/OS Container Extensions platform CURL implementation does not work as expected. If zCX platform detected, add flag to adjust.
  zPlatform= docker system info | grep platform
  echo $zPlatform
  if [[ $zPlatform -eq "zOS" ]]; then
      # -k suppresses the security check allowing the curl download to complete. This check should be removed when 
      # the CURL implementation on z/OS Container Extensions is fixed properly in a future release.
      CURL_FIX_ZCX=" -k "
  fi
else
  echo " "
  echo " FATAL: Aborting. $ARCH is not a suppported platform for building the FHE Toolkit."
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
if [ $ARCH == "s390x" ]; then
  if [[ "$platform" = "ubuntu" ]]; then
      build="UBUNTU"
      platform="ubuntu"
      release="20.04"
  elif [[ "$platform" = "fedora" ]]; then
      build="FEDORA"
      platform="fedora"
      release="32"
  elif [[ "$platform" = "alpine" ]]; then
      build="ALPINE"
      platform="alpine"
      release="3.12"  
  else
      echo " "
      echo "  Invalid platform: $platform Please specify a supported platform: <fedora>, <ubuntu>"
      echo " "
      print_usage
      exit -5
  fi
else
  if [ $ARCH == "amd64" ]; then
    if [[ "$platform" = "fedora" ]]; then
      build="FEDORA"
      platform="fedora"
      release="32"
    elif [[ "$platform" = "centos" ]]; then
      build="CENTOS"
      platform="centos"
      release="8"
    elif [[ "$platform" = "ubuntu" ]]; then
      build="UBUNTU"
      platform="ubuntu"
      release="20.04"
    elif [[ "$platform" = "alpine" ]]; then
      build="ALPINE"
      platform="alpine"
      release="3.12"  
    else
      echo " "
      echo "  Invalid platform: $platform Please specify a support platform: <fedora>, <centos>, <ubuntu>"
      echo " "
      print_usage
      exit -6
    fi
  fi
fi

platform_tag=$platform-$ARCH

################################################################
# Check for required utilities: curl, git, tar, docker
if ! [ -x "$(command -v curl)" ]
then
  echo " "
  echo " FATAL: The curl command line utility is not installed. Please install curl to continue."
  echo " "
  exit -6
elif ! [ -x "$(command -v tar)" ]
then
  echo " "
  echo " FATAL: The tar command line utility is not installed. Please install tar to continue."
  echo " "
  exit -7
elif ! [ -x "$(command -v docker)" ]
then
  echo " "
  echo " FATAL: The docker utility is not installed. Please install docker to continue."
  echo " "
  exit -8
fi


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
echo " Building FHE Toolkit $HElib_version for Linux"
echo " Platform $build-$ARCH:$release "
echo ""
echo "==============================================================="

echo "==============================================================="
echo ""
echo " Phase 1: Downloading dependencies locally"
echo ""
echo "==============================================================="

#
# Fetch external dependencies
#
cd $BASEDIR/DEPENDENCIES
get_NTL $NTL_version
get_HElib $HElib_version
get_Boost $Boost_version $Boost_filename


echo "==============================================================="
echo ""
echo " Phase 2: Building Base Environment $flags"
echo ""
echo "==============================================================="
PlatformRelease=${platform}:${release}
HElib_tag=sys-ibm-fhe-team-zcx-conda-docker-local.artifactory.swg-devops.com/encrypt-poc:1.0
FHEKit_tag=local/fhe-toolkit-${platform_tag}:${HElib_version}
FHEKit_tag_latest=local/fhe-toolkit-${platform_tag}
FHEKit_latest=local/fhe-toolkit-${platform}

#
# Change to $BASEDIR directory and build an HElib Docker image
#
cd $BASEDIR
echo " "
echo "name of the docker image "
echo "$HElib_tag"
echo " "
if ! docker build -f ./Dockerfile.$build.HElib -t $HElib_tag --build-arg PlatformRelease=$PlatformRelease . $flags
then
  echo " "
  echo " FATAL: Failure building HElib base container. Please check that you have a working Docker installation"
  echo " in your system and the necessary user privileges to run docker commands."
  echo " "
  exit 10
fi


#
# Build FHE Tookit Docker image
#
cd $BASEDIR
if ! docker build -f ./Dockerfile.$build-$ARCH.Toolkit -t $FHEKit_tag --build-arg HElib_tag=$HElib_tag . $flags
then
  echo " "
  echo " FATAL: Failure building Toolkit container. Please check that you have a working Docker installation"
  echo " in your system and the necessary user privileges to run docker commands, and that the HElib base "
  echo " container built correctly. This check can be done by issueing the command `docker images`"
  echo " "
  exit -11
else
  echo " "
  echo " Toolkit build $FHEKit_tag completed."
  echo " "
fi

#
# Tag local build as local/latest
#
if ( ! docker tag $FHEKit_tag $FHEKit_tag_latest || ! docker tag $FHEKit_tag $FHEKit_latest )
then
  echo " "
  echo " Failed to re-tag the toolkit $FHEKit_tag "
  echo " Please check for error messages above."
  echo " "
  exit -12
fi

#
# Clean up temporary containers
#
docker rmi $HElib_tag
if [ $? -ne 0 ]
then
  echo " "
  echo " WARNING: Failed to Remove $HElib_tag"
  echo " Please check for matching docker image below."
  docker images --all
  echo " "
fi
docker rmi $PlatformRelease
if [ $? -ne 0 ]
then
  echo " "
  echo " WARNING: Failed to Remove $PlaformRelease"
  echo " Please check for matching docker image below."
  docker images --all
  echo " "
fi
#Throwing in test comment so it forces rebuild
#makeing other change so it shows somethning is differetn

echo "==============================================================="
echo ""
echo " Phase 3: Setup Toolkit Persistent Working Directory"
echo ""
echo "==============================================================="
cd $BASEDIR
./PersistData.sh $FHEKit_latest
echo " "
echo " Toolkit Environment build is complete."
echo " "
