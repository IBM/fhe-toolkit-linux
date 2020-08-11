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
# Pull latest from the FHE repo, master branch
git checkout master
# Build the Docker image for CentOS
./BuildDockerImage.sh centos
# Shut everything down before we start
./StopToolkit.sh
# Run the toolkit container based on the CentOS image
./RunToolkit.sh -l -s centos
# Test sample runs as expected
docker exec local-fhe-toolkit-centos /bin/bash -c " \
    cd /opt/IBM/FHE-Workspace; \
    mkdir build; cd build; \
    cmake ../examples/BGV_country_db_lookup;\
    make;\
    cd ..;\
    ./examples/BGV_country_db_lookup/runtest.sh;"
# Shut everything down 
./StopToolkit.sh
