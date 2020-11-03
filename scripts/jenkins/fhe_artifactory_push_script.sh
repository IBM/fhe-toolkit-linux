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


source ConfigConstants.sh
ARTE_USER=$1
ARTE_PWD=$2
BUILD_TYPE=$3
PR_NUM=$4
LINUX_VERSION=$5

NOW=$(date +'%m-%d-%Y')
NIGHTLY_SUFFIX="nightly-${NOW}"
VERSION="$HElib_version.$TOOLKIT_VERSION"
PR_SUFFIX="pr-${PR_NUM}"
ARTE_URL=""

#Login to Artifactory using the fhe user
echo "DOCKER LOGIN"
#docker login -u $ARTE_USER -p $ARTE_PWD "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"
echo $ARTE_PWD | docker login -u $ARTE_USER --password-stdin "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"

#If this is a s390 machine, then tag and push for S390
if [[ "$BUILD_TYPE" == "S390" ]]; then
    echo "Tagging for S390"
    ARTE_URL="sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/$LINUX_VERSION/fhe-toolkit-$LINUX_VERSION-s390x:$VERSION-$PR_NUM"
    #Tag the docker build for storage in Artifactory
    echo "tagging it"
    #Push and save the newly tagged build in Artifactory
    docker push $ARTE_URL
    echo "pushing it"
    BUILD_TYPE="s390x"
else
    #Tag the docker build for storage in Artifactory
    ARTE_URL="sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/$LINUX_VERSION/fhe-toolkit-$LINUX_VERSION-amd64:$VERSION-$PR_NUM"
    echo "tagging it"
    #Push and save the newly tagged build in Artifactory
    docker push $ARTE_URL
    BUILD_TYPE="amd64"
    echo "pushing it"
fi