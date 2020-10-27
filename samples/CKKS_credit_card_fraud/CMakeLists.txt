#
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
#

project(CKKS_credit_card_fraud)
ENABLE_LANGUAGE(CXX)

# We need a recent cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# We rely on HELib...
find_package(helib ${HELIB_VERSION} REQUIRED)

# We rely on HDF5 for data access...
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIR})

# We rely on Boost C++
find_package(Boost REQUIRED COMPONENTS filesystem)

set( CMAKE_CXX_FLAGS "-Werror -fopenmp" )

# Build the credit_card_fraud object from the C++ source files.
add_executable(CKKS_credit_card_fraud helib_main.cpp ClientServer.cpp)

# Link credit_card_fraud, hdf5, helib, boost, and hedge_hebase
target_link_libraries(CKKS_credit_card_fraud -lhedge_hebase ${HDF5_LIBRARIES} helib ${Boost_LIBRARIES})