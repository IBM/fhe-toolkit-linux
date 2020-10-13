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

# Go to the top level of the Repo
pushd ../../
set -x 
set -u
set -e

ARTE_USER=$1
ARTE_PWD=$2

# Pull latest from the FHE repo, master branch
git checkout s390-build-tests
# Build the Docker image for Fedora
./BuildDockerImage.sh fedora
# Shut everything down before we start
./StopToolkit.sh
# Run the toolkit container based on the Fedora image
./RunToolkit.sh -l -s fedora
# Test sample runs as expected
docker exec local-fhe-toolkit-fedora /bin/bash -c " \
    cd /opt/IBM/FHE-Workspace; \
    mkdir build; cd build; \
    cmake ../examples/BGV_country_db_lookup;\
    make;\
    cd ..;\
    chmod 755 examples/BGV_country_db_lookup/runtest.sh;\
    ./examples/BGV_country_db_lookup/runtest.sh;"
# Shut everything down 
./StopToolkit.sh

#Login to Artifactory using the fhe user
echo "DOCKER LOGIN"
#This works but its an alternate login version
#docker login -u $ARTE_USER -p $ARTE_PWD "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"
echo $ARTE_PWD | docker login -u $ARTE_USER --password-stdin "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com"
#Tag the docker build for storage in Artifactory
docker tag "local/fhe-toolkit-fedora-amd64:latest" "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/fedora/fhe-toolkit-fedora-amd64:v1.0.2-latest"
echo "tagging it"
#Push and save the newly tagged build in Artifactory
docker push "sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/fedora/fhe-toolkit-fedora-amd64:v1.0.2-latest"
echo "pushing it"
