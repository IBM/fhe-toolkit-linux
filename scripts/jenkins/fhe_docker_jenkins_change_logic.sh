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



#make new script, to contain logic to figure out which script to run

ARTE_USER=$1
ARTE_PWD=$2

GIT_LOG=$(git log -1 --name-only)
echo $GIT_LOG
CENT='CENTOS'
FED='FEDORA'
UBU='UBUNTU'
BUILD_ALL='BuildDockerImage'
RUN_ALL='RunToolkit'
PERSIST='PersistData'
SAMPLES='samples'
DEPENDENCIES='DEPENDENCIES'
VERSION_CHANGES='ConfigConstants'



#So it any changes were made to the base docker, build it all
#TODO: find out how to check if something has changed with the HELib repo and it needs to be changed
#      could be taken care of by just a change in the build docker image script because if something is going to change
#      I think that means that the version of the repo will be updated
#if [ "$GIT_LOG" == *"$BUILD_ALL"* ] || [ "$GIT_LOG" == *"$RUN_ALL"* ] || [ "$GIT_LOG" == *"$PERSIST"* ] || [ "$GIT_LOG" == *"$SAMPLES"* ] || [ "$GIT_LOG" == *"$VERSION_CHANGES"* ] || [ "$GIT_LOG" == *"$DEPENDENCIES"* ]; then
if [[ "$GIT_LOG" == *"$VERSION_CHANGES"*  || "$GIT_LOG" == *"$PERSIST"* ]]; then 
  echo "CHANGES WERE MADE SO IGNORE THE REST"
  #./fhe_docker_jenkins_trigger_builds_ubuntu.sh $ARTE_USER $ARTE_PWD
  #./fhe_docker_jenkins_trigger_builds_fedora.sh $ARTE_USER $ARTE_PWD
  #./fhe_docker_jenkins_trigger_builds_centos.sh $ARTE_USER $ARTE_PWD
else
    if [[ "$GIT_LOG" == *"$FED"* ]]; then
        echo "REBUILD FEDORA"
        ./fhe_docker_jenkins_trigger_builds_fedora.sh $ARTE_USER $ARTE_PWD
    fi
    if [[ "$GIT_LOG" == *"$UBU"* ]]; then
        echo "REBULD UBUNTU"
        ./fhe_docker_jenkins_trigger_builds_ubuntu.sh $ARTE_USER $ARTE_PWD
    fi
    if [[ "$GIT_LOG" == *"$CENT"* ]]; then
        echo "REBULD CENTOS"
        ./fhe_docker_jenkins_trigger_builds_centos.sh $ARTE_USER $ARTE_PWD
    else 
      echo "DID NOT DETECT ANY CHANGES, SO NO RE-BUILDS"
    fi

fi



