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

# Retrieve Container ID and Image Name of running toolkit
CONTAINER_ID=$(docker ps -a -q --filter name=fhe-toolkit --format="{{.ID}}")
IMAGE_NAME=$(docker ps -a -q --filter name=fhe-toolkit --format="{{.Names}} ({{.Image}})")

if [ -z "$CONTAINER_ID" ]
then
  echo "No FHE Toolkit Docker Container currently active"
  exit 0
else
  echo " Stopping Docker Container: $IMAGE_NAME : $CONTAINER_ID ..."
  docker rm  $(docker stop $CONTAINER_ID) > /dev/null 2>&1
  echo " Done"
fi

