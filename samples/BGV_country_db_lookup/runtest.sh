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

queries=('Albania:Tirana'
         'Austria:Vienna'
         'Belarus:Minsk'
         'Southampton:Country'
        )

# Number of queries
rt=${#queries[@]}

for query in "${queries[@]}"; do
  # Capture the result value for comparison
  value=$( $PWD/build/BGV_country_db_lookup <<< "${query%:*}" | awk '/Query result:/{ print $3 }' )

  echo "${query%:*} gives '$value'"
  # If the query result matches then decrement the return value rt
  if [ "$value" = "${query#*:}" ]; then
    rt=$((rt-1))
  fi
done

# This will return the overall pass or fail
exit "$rt"
