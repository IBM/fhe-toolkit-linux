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



SLACK_HOOK=$1
BUILD_KIND=$2
BUILD_TYPE=$3
ARTE_URL=$4
COMMIT_URL=""

generate_post_message()
{
 cat <<EOF
 {
  "text":"A new build of $BUILD_KIND for $BUILD_TYPE based on GitHub commit: <$COMMIT_URL|$COMMIT_NUMBER >, passed all the tests and is now available for docker pull at <$ARTE_URL>"
 }
EOF
}

get_commit_url()
{
  #Make HTTPS
  SUFFIX=".git"
  BASE_URL="https://github.com/IBM/fhe-toolkit-linux"
  GIT_BASE=${BASE_URL%"$SUFFIX"}
  origin_head=$(git ls-remote --heads $(git config --get remote.origin.url) | grep "refs/heads/master" | cut -f 1)
  COMMIT_URL=$GIT_BASE"/commit/$origin_head"
  echo $COMMIT_URL
}



COMMIT_NUMBER="$(git log -1 --pretty=format:"%h")"


get_commit_url
echo $(generate_post_message)
curl -X POST -H 'Content-type: application/json' --data "$(generate_post_message)" $SLACK_HOOK

