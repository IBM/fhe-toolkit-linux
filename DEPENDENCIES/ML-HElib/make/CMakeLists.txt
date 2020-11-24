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


project(hedge_hebase VERSION ${MLHElib_version} LANGUAGES CXX)

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

# Hedge main files
set(IMPL_SOURCES
../src/hedge/hebase/impl/AbstractCiphertext.cpp
../src/hedge/hebase/impl/AbstractEncoder.cpp
../src/hedge/hebase/impl/AbstractFunctionEvaluator.cpp
../src/hedge/hebase/impl/AbstractPlaintext.cpp)

set(HEBASE_SOURCES ${IMPL_SOURCES} 
../src/hedge/hebase/AlwaysAssert.cpp
../src/hedge/hebase/BitwiseEvaluator.cpp
../src/hedge/hebase/CTile.cpp
../src/hedge/hebase/Encoder.cpp
../src/hedge/hebase/FileUtils.cpp
../src/hedge/hebase/NativeFunctionEvaluator.cpp
../src/hedge/hebase/HeContext.cpp
../src/hedge/hebase/HeTraits.cpp
../src/hedge/hebase/PTile.cpp
../src/hedge/hebase/HedgeTimer.cpp)

set(HEDGE_HELIB_SOURCES 
../src/hedge/hebase/helib/HelibBgvCiphertext.cpp
../src/hedge/hebase/helib/HelibBgvContext.cpp
../src/hedge/hebase/helib/HelibBgvEncoder.cpp
../src/hedge/hebase/helib/HelibBgvPlaintext.cpp
../src/hedge/hebase/helib/HelibBgvNativeFunctionEvaluator.cpp
../src/hedge/hebase/helib/HelibCkksCiphertext.cpp
../src/hedge/hebase/helib/HelibCkksContext.cpp
../src/hedge/hebase/helib/HelibCkksEncoder.cpp
../src/hedge/hebase/helib/HelibCkksPlaintext.cpp
../src/hedge/hebase/helib/HelibCiphertext.cpp
../src/hedge/hebase/helib/HelibConfig.cpp
../src/hedge/hebase/helib/HelibContext.cpp)

set(SIMPLE_NN_SOURCES ../src/hedge/simple_nn/DoubleMatrix.cpp
../src/hedge/simple_nn/DoubleMatrixArray.cpp
../src/hedge/simple_nn/h5Parser.cpp 
../src/hedge/simple_nn/TrainingSetPlain.cpp
../src/hedge/simple_nn/SimpleNeuralNetPlain.cpp
../src/hedge/simple_nn/SimpleNeuralNet.cpp
../src/hedge/simple_nn/CipherMatrix.cpp
../src/hedge/simple_nn/CipherMatrixEncoder.cpp
../src/hedge/simple_nn/SimpleSquareActivationLayer.cpp
../src/hedge/simple_nn/SimpleLayer.cpp
../src/hedge/simple_nn/SimpleFcLayer.cpp
../src/hedge/simple_nn/SimpleFcPlainLayer.cpp
../src/hedge/simple_nn/SimpleSquareActivationPlainLayer.cpp) 


set(HEBASE_TESTS
../test/unittest/hebase/CTileTest.cpp
../test/unittest/hebase/EncoderTest.cpp
../test/unittest/hebase/NativeFunctionEvaluatorTest.cpp
../test/unittest/hebase/HeContextTest.cpp
../test/unittest/hebase/PTileTest.cpp)


# Main library
add_library(hedge_hebase STATIC ${HEBASE_SOURCES} ${SIMPLE_NN_SOURCES} ${HEDGE_HELIB_SOURCES})
target_link_libraries(hedge_hebase helib ${Boost_LIBRARIES} Boost::headers)

add_executable(helib_tests ../test/unittest/helib_tests.cpp ../test/util/TestUtils.cpp ${HEBASE_TESTS})
target_link_libraries(helib_tests hedge_hebase  helib Boost::headers gtest_main)
SET_TARGET_PROPERTIES(helib_tests PROPERTIES LINK_FLAGS -pthread)
target_link_libraries(helib_tests ${Boost_LIBRARIES})

add_executable(helib_bgv_tests ../test/unittest/helib_bgv_tests.cpp ../test/util/TestUtils.cpp ../test/unittest/hebase/CTileIntTests.cpp)
target_link_libraries(helib_bgv_tests hedge_hebase  helib Boost::headers gtest_main)
SET_TARGET_PROPERTIES(helib_bgv_tests PROPERTIES LINK_FLAGS -pthread)
target_link_libraries(helib_bgv_tests ${Boost_LIBRARIES})


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
