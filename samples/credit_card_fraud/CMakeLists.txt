# Copyright (C) 2020 IBM Corp.
# This program is Licensed under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#   http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. See accompanying LICENSE file.


project(credit_card_fraud)
ENABLE_LANGUAGE(CXX)

# We need a recent cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# We rely on HELib...
find_package(helib 1.0.0 REQUIRED)

# We rely on HDF5 for data access...
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIR})

# We rely on Boost C++
find_package(Boost REQUIRED COMPONENTS filesystem)

# Build the credit_card_fraud object from credit_card_fraud.cpp entry point
add_executable(credit_card_fraud credit_card_fraud.cpp)

INCLUDE_DIRECTORIES(/usr/local/include/simple_ml_helib)

# Link credit_card_fraud, hdf5, helib, boost, and simple_ml_helib
target_link_libraries(credit_card_fraud -lsimple_ml_helib ${HDF5_LIBRARIES} helib ${Boost_LIBRARIES})
