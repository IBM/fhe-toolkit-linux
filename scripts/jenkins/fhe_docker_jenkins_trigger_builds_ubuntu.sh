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

# Pull latest from the FHE repo, master branch
git checkout master
# Build the Docker image for Ubuntu
./BuildDockerImage.sh ubuntu
# Shut everything down before we start
./StopToolkit.sh
# Run the toolkit container based on the Ubuntu image
./RunToolkit.sh -l -s ubuntu
# Test sample runs as expected
docker exec local-fhe-toolkit-ubuntu /bin/bash -c " \
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

#Login to Artifactory using the fhe user
echo "DOCKER LOGIN"
#docker login -u $ARTE_USER -p $ARTE_PWD "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"
echo $ARTE_PWD | docker login -u $ARTE_USER --password-stdin "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"

#If this is a s390 machine, then tag and push for S390
if [[ "$BUILD_TYPE" == "S390" ]]; then
    echo "Tagging for S390"
    #Tag the docker build for storage in Artifactory
    docker tag "local/fhe-toolkit-ubuntu-s390x:latest" "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/ubuntu/fhe-toolkit-ubuntu-s390x:$VERSION-$NIGHTLY_SUFFIX"
    echo "tagging it"
    #Push and save the newly tagged build in Artifactory
    docker push "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/ubuntu/fhe-toolkit-ubuntu-s390x:$VERSION-$NIGHTLY_SUFFIX"
    echo "pushing it"
else
#This is an x86 machine, so tag and push for x86
    echo "Tagging for x86"
    #Tag the docker build for storage in Artifactory
    docker tag "local/fhe-toolkit-ubuntu-amd64:latest" "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/ubuntu/fhe-toolkit-ubuntu-amd64:$VERSION-$NIGHTLY_SUFFIX"
    echo "tagging it"
    #Push and save the newly tagged build in Artifactory
    docker push "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/ubuntu/fhe-toolkit-ubuntu-amd64:$VERSION-$NIGHTLY_SUFFIX"
    echo "pushing it"
fi



