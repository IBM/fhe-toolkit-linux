#!/bin/bash

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

country_capitals=('Albania:Tirana' 'Austria:Vienna' 'Belarus:Minsk' 'Southampton:Country')

# Number of queries
rc=${#country_capitals[@]}

for country_capital in "${country_capitals[@]}"; do
  # Capture the result value for comparison
  query="${country_capital%:*}"
  capital=$( $PWD/build/BGV_country_db_lookup <<< $query | awk '/Query result:/{ print $3 }' )

  echo "$query returns '$capital'"
  if [ "$capital" = "${country_capital#*:}" ]; then 
    let rc--;
  else
    echo "Test failed.."
    echo "Expected: $country_capital"
    echo "Actual: $query:$capital"
    echo ""
  fi
done

# This will return the overall pass or fail
echo $rc
exit $rc