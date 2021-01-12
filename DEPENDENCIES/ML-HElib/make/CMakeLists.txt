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

project(MLHElib VERSION ${MLHElib_version} LANGUAGES CXX)

# This policy allows setting the xxxx_ROOT setting to choose another location
# for libraries (for example, HDF5_ROOT).
cmake_policy(SET CMP0074 NEW)

set(CMAKE_CXX_STANDARD 17)

include_directories(../src/)
include_directories(../test/util/)

# Executable will be in bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# Library will be in lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set( CMAKE_CXX_FLAGS "-Werror -fopenmp" )

# Import Boost
find_package(Boost ${Boost_version} EXACT REQUIRED COMPONENTS filesystem system thread)

# main files
set(IMPL_SOURCES
../src/helayers/hebase/impl/AbstractCiphertext.cpp
../src/helayers/hebase/impl/AbstractEncoder.cpp
../src/helayers/hebase/impl/AbstractFunctionEvaluator.cpp
../src/helayers/hebase/impl/AbstractPlaintext.cpp)

set(HEBASE_SOURCES ${IMPL_SOURCES} 
../src/helayers/hebase/AlwaysAssert.cpp
../src/helayers/hebase/BitwiseEvaluator.cpp
../src/helayers/hebase/CTile.cpp
../src/helayers/hebase/Encoder.cpp
../src/helayers/hebase/FileUtils.cpp
../src/helayers/hebase/NativeFunctionEvaluator.cpp
../src/helayers/hebase/HeContext.cpp
../src/helayers/hebase/HeTraits.cpp
../src/helayers/hebase/PTile.cpp
../src/helayers/hebase/HelayersTimer.cpp
../src/helayers/hebase/utils/JsonWrapper.cpp
../src/helayers/hebase/utils/JsonSubtree.cpp
../src/helayers/hebase/utils/HelayersConfig.cpp
)

set(HEBASE_HELIB_SOURCES 
../src/helayers/hebase/helib/HelibBgvCiphertext.cpp
../src/helayers/hebase/helib/HelibBgvContext.cpp
../src/helayers/hebase/helib/HelibBgvEncoder.cpp
../src/helayers/hebase/helib/HelibBgvPlaintext.cpp
../src/helayers/hebase/helib/HelibBgvNativeFunctionEvaluator.cpp
../src/helayers/hebase/helib/HelibCkksCiphertext.cpp
../src/helayers/hebase/helib/HelibCkksContext.cpp
../src/helayers/hebase/helib/HelibCkksEncoder.cpp
../src/helayers/hebase/helib/HelibCkksPlaintext.cpp
../src/helayers/hebase/helib/HelibCiphertext.cpp
../src/helayers/hebase/helib/HelibConfig.cpp
../src/helayers/hebase/helib/HelibContext.cpp)

set(SIMPLE_NN_SOURCES ../src/helayers/simple_nn/DoubleMatrix.cpp
../src/helayers/simple_nn/DoubleMatrixArray.cpp
../src/helayers/simple_nn/h5Parser.cpp 
../src/helayers/simple_nn/TrainingSetPlain.cpp
../src/helayers/simple_nn/SimpleNeuralNetPlain.cpp
../src/helayers/simple_nn/SimpleNeuralNet.cpp
../src/helayers/simple_nn/CipherMatrix.cpp
../src/helayers/simple_nn/CipherMatrixEncoder.cpp
../src/helayers/simple_nn/SimpleSquareActivationLayer.cpp
../src/helayers/simple_nn/SimpleLayer.cpp
../src/helayers/simple_nn/SimpleFcLayer.cpp
../src/helayers/simple_nn/SimpleFcPlainLayer.cpp
../src/helayers/simple_nn/SimpleSquareActivationPlainLayer.cpp) 


set(HEBASE_TESTS
../test/unittest/hebase/CTileTest.cpp
../test/unittest/hebase/EncoderTest.cpp
../test/unittest/hebase/NativeFunctionEvaluatorTest.cpp
../test/unittest/hebase/HeContextTest.cpp
../test/unittest/hebase/PTileTest.cpp
../test/unittest/hebase/UtilsTest.cpp)


# Main library
add_library(mlhelib STATIC ${HEBASE_SOURCES} ${SIMPLE_NN_SOURCES} ${HEBASE_HELIB_SOURCES})
target_link_libraries(mlhelib helib ${Boost_LIBRARIES} Boost::headers)

add_executable(mlhelib_tests ../test/unittest/mlhelib_tests.cpp ../test/util/TestUtils.cpp ${HEBASE_TESTS})
target_link_libraries(mlhelib_tests mlhelib  helib Boost::headers gtest_main)
SET_TARGET_PROPERTIES(mlhelib_tests PROPERTIES LINK_FLAGS -pthread)
target_link_libraries(mlhelib_tests ${Boost_LIBRARIES})

add_executable(mlhelib_bgv_tests ../test/unittest/mlhelib_bgv_tests.cpp ../test/util/TestUtils.cpp ../test/unittest/hebase/CTileIntTests.cpp)
target_link_libraries(mlhelib_bgv_tests mlhelib  helib Boost::headers gtest_main)
SET_TARGET_PROPERTIES(mlhelib_bgv_tests PROPERTIES LINK_FLAGS -pthread)
target_link_libraries(mlhelib_bgv_tests ${Boost_LIBRARIES})


#### Find dependencies

# Import HDF5
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIR})

# Import helib
find_package(helib)

# the below googletest-related content, as well as the content of CMakeLists.txt.in file,
# was copied from the following page:
# https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
