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
pushd `git rev-parse --show-toplevel`
pwd
set -x 
set -u
set -e

remove_toolkit() {
    #Remove any previous lingering files from the last build in case it wasn't removed
    if [ -d "fhe-toolkit-linux" ]; then
        rm -rf fhe-toolkit-linux
    fi
} 

remove_toolkit


H_FILES='.h'
CPP_FILES='.cpp'
GIT_LOG=$(git log --since="24 hours ago" --name-only)
echo $GIT_LOG
COMMIT_NUMBER=$(git log -1 --pretty=format:"%h")

source ConfigConstants.sh
VERSION="$HElib_version.$TOOLKIT_VERSION"

#Check to see if the .h files or the .cpp files were changed recently otherwise do nothing
if [[ "$GIT_LOG" == *"$H_FILES"* || "$GIT_LOG" == *"$CPP_FILES"* ]]; then 
    echo "CHANGES WERE MADE SO RE-GENERATE THE DOCS"
    # Checkout specifically the gh-pages branch, so we can push our docs to it
    git clone -b gh-pages --single-branch git@github.com:IBM/fhe-toolkit-linux.git
    #CD into the new Toolkit linux directory
    cd fhe-toolkit-linux
    #Call Write ML Helib Api Docs script so we generate docs and write them to the html folder
    ./RenderMLhelibAPI.sh local/fhe-toolkit-ubuntu html $VERSION
    ./RenderHELibDocs.sh local/fhe-toolkit-ubuntu html $VERSION

    echo `git add -A && git commit -m "Re-Generated Docs from this commit {$COMMIT_NUMBER}"`
    echo `git push origin gh-pages`
    pushd ../
    pwd
    remove_toolkit
else
    echo "NO CHANGES TO CODE SO NO Need to REGENEATE DOCS"
fi




