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


# To compile with HElib add "-Dhelib_DIR=<helib install prefix>/share/cmake/helib" to the 
# cmake instruction

cmake_minimum_required(VERSION 3.10)


project(simple_ml_helib VERSION 0.1.0 LANGUAGES CXX)

include_directories(./src/)

# Executable will be in bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# Library will be in lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set( CMAKE_CXX_FLAGS "-Werror" )

set(NN_SOURCES
  src/DoubleMatrix.cpp
  src/DoubleMatrixArray.cpp
  src/h5Dumper.cpp
  src/h5Parser.cpp
  src/SimpleTrainingSet.cpp
  src/Layer.cpp
  src/FcLayer.cpp
  src/SimpleFcLayer.cpp
  src/SimpleSquareActivationLayer.cpp
  src/SimpleNeuralNetPlain.cpp
  src/SimpleNeuralNet.cpp)

set(HELIB_WRAP_SOURCES
  src/HelibContext.cpp
  src/HelibConfig.cpp
  src/HelibCkksContext.cpp
  src/HelibCiphertext.cpp
  src/HelibCkksCiphertext.cpp
  src/HelibCkksPlaintext.cpp
  src/HelibCkksEncoder.cpp)

set(LIB_SOURCES ${NN_SOURCES} ${HELIB_WRAP_SOURCES}
  src/AbstractCiphertext.cpp
  src/SimpleTimer.cpp
  src/HeContext.cpp
  src/AlwaysAssert.cpp
  src/CTile.cpp
  src/PTile.cpp
  src/AbstractEncoder.cpp
  src/Encoder.cpp
  src/FileUtils.cpp
  src/CipherMatrix.cpp
  src/CipherMatrixEncoder.cpp)

# Main library
add_library(simple_ml_helib STATIC ${LIB_SOURCES})
target_link_libraries(simple_ml_helib helib ${Boost_LIBRARIES} Boost::headers)

#### Find dependencies

# Import HDF5
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIR})

# Import helib
find_package(helib)

# Import Boost
find_package(Boost 1.72.0 EXACT REQUIRED COMPONENTS filesystem system thread)
