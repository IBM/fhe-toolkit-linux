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

. ./ConfigConstants.sh

cd DEPENDENCIES

if [ -f "ntl-$NTL_version.tar.gz" ]; then 
  echo "Removing"
  rm -rf "ntl-$NTL_version.tar.gz"
  rm -rf "NTL"
  echo "Removing NTL Libs"
else
  echo " NO NTL found, nothing to remove"
fi

if [ -f "HElib-$HElib_version.tar.gz" ]; then  
  rm -rf "HElib-$HElib_version.tar.gz"
  rm -rf "HElib"
  echo "Removing HELib"
else
   echo " NO HELib found, nothing to remove"
fi

cd ../

if [ -d "FHE-Toolkit-Workspace" ]; then 
  rm -rf "FHE-Toolkit-Workspace"
  echo "Removing FHE-Toolkit-Workspace"
else
   echo " NO FHE-Toolkit-Workspace found, nothing to remove"
fi

exit 0

