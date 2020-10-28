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

# Pull latest from the FHE repo, master branch
git checkout master
# Build the Docker image for Alpine
./BuildDockerImage.sh alpine
# Shut everything down before we start
./StopToolkit.sh
# Run the toolkit container based on the Alpine image
./RunToolkit.sh -l -s alpine
# Test sample runs as expected
docker exec local-fhe-toolkit-alpine /bin/bash -c " \
    cd /opt/IBM/FHE-Workspace; \
    mkdir build; cd build; \
    cmake ../examples/BGV_country_db_lookup;\
    make;\
    cd ..;\
    chmod 755 examples/BGV_country_db_lookup/runtest.sh;\
    ./examples/BGV_country_db_lookup/runtest.sh;"
# Shut everything down 
./StopToolkit.sh


NOW=$(date +'%m-%d-%Y')
NIGHTLY_SUFFIX="nightly-${NOW}"
VERSION="$HElib_version.$TOOLKIT_VERSION"
ARTE_URL=""

#Login to Artifactory using the fhe user
echo "DOCKER LOGIN"
#docker login -u $ARTE_USER -p $ARTE_PWD "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"
echo $ARTE_PWD | docker login -u $ARTE_USER --password-stdin "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"

#If this is a s390 machine, then tag and push for S390
if [[ "$BUILD_TYPE" == "S390" ]]; then
    echo "Tagging for S390"
    ARTE_URL="sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/alpine/fhe-toolkit-alpine-s390x:$VERSION-$NIGHTLY_SUFFIX"
    #Tag the docker build for storage in Artifactory
    docker tag "local/fhe-toolkit-alpine-s390x:latest" $ARTE_URL
    echo "tagging it"
    #Push and save the newly tagged build in Artifactory
    docker push $ARTE_URL
    echo "pushing it"
    BUILD_TYPE="s390x"
else
    #Tag the docker build for storage in Artifactory
    ARTE_URL="sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/alpine/fhe-toolkit-alpine-amd64:$VERSION-$NIGHTLY_SUFFIX"
    docker tag "local/fhe-toolkit-alpine-amd64:latest" $ARTE_URL
    echo "tagging it"
    #Push and save the newly tagged build in Artifactory
    docker push $ARTE_URL
    BUILD_TYPE="amd64"
    echo "pushing it"
fi

#Make A Notification in the Slack Channel about a new artifact in the repo
pushd scripts/jenkins
./fhe_artifactory_notification_script.sh $SLACK_HOOK "Alpine" $BUILD_TYPE $ARTE_URL
