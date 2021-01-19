#/bin/bash 


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


# Navigate to the top level of the Repo
pushd ../../
set -x 
set -u
set -e

source ConfigConstants.sh
ARTE_USER=$1
ARTE_PWD=$2
BUILD_TYPE=$3
SLACK_HOOK=$4
PR_NUMBER=$5

test_toolkit() 
{
    LINUX_FLAVOR=$1
    echo "WE Are here"
    pwd
    # Build the Docker image for Ubuntu
    ./BuildDockerImage.sh $LINUX_FLAVOR
    # Shut everything down before we start
    ./StopToolkit.sh
    # Run the toolkit container based on the Ubuntu image
    ./RunToolkit.sh -l -s $LINUX_FLAVOR
    LOCAL_DOCKER_IMAGE="local-fhe-toolkit-$LINUX_FLAVOR"
    # Test sample runs as expected
    # Run ML-HElib unit tests and samples tests
    docker exec $LOCAL_DOCKER_IMAGE /opt/IBM/FHE-distro/ML-HElib/test_mlhelib.sh

    # Shut everything down 
    ./StopToolkit.sh

    pushd scripts/jenkins
    ./fhe_artifactory_push_script.sh $ARTE_USER $ARTE_PWD $BUILD_TYPE $PR_NUMBER $LINUX_FLAVOR
    pushd ../../

}

if [[ "$BUILD_TYPE" == "S390" ]]; then
     echo 'S390 Stuff $BUILD_TYPE'
     test_toolkit "ubuntu"
     test_toolkit "fedora"
     test_toolkit "alpine"
   
else 
     test_toolkit "ubuntu"
     test_toolkit "fedora"
     test_toolkit "alpine"
     test_toolkit "centos"
fi

 
