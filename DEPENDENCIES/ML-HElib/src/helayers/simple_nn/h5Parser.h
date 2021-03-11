/*
 * MIT License
 *
 * Copyright (c) 2020 International Business Machines
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PARSER_H_
#define PARSER_H_

#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>

#include <vector>
#include "H5Cpp.h"

namespace helayers {

#define BIAS_RANK 1
#define DENSE_RANK 2
#define FILTER_RANK 4

/** A simple utility class for loading data from h5 files */
class H5Parser
{
protected:
  std::string file_name;
  H5::H5File file;

public:
  /// Constructor.
  /// @param[in] f file to read
  H5Parser(const std::string& f);

  virtual ~H5Parser();

  /// Reads the scalar from the given path into "val".
  /// @param[in] path               The path to read from.
  /// @param[out] val               This will contain the read value.
  /// @throw   invalid_arguement    if the given path contain a non-scalar
  ///                               tensor.
  void readData(const std::string& path, double& val) const;

  /// Reads the (flattened) tensor from the given path into "vals" , and stores
  /// its original dimensions in "dims".
  /// @param[in] path    The path to read from.
  /// @param[out] vals   This vector will contain The tensor read from the given
  ///                    path, flattened into 1D vector.
  /// @param[out] dims   The original dimensions of the tensor in the given
  ///                    path.
  void readData(const std::string& path,
                std::vector<double>& vals,
                std::vector<int>& dims) const;

  /// Reads the tensor from the given path into "vals".
  /// @param[in] path     The path to read from
  /// @param[out] vals    This tensor will contain the tensor read from the
  ///                     given path.
  void readData(const std::string& path,
                boost::numeric::ublas::tensor<double>& vals) const;

  /// Loads fully-connected layer bias from a given path inside file.
  std::vector<double> parseBias(const std::string& path) const;

  /// Loads fully-connected layer weights from a given path inside file.
  std::vector<std::vector<double>> parseFC(const std::string& path) const;

  /// Loads convolution layer filters from a given path inside file.
  std::vector<std::vector<std::vector<double>>> parseFilters(
      const std::string& path) const;

  /// Checks if given name exists in file
  bool objectExists(const std::string& name) const;

  /// Returns underlying H5File object
  inline H5::H5File& getFile() { return file; }

  // bool sameHierarchy(const H5Parser& other) const;
};
} // namespace helayers

#endif /* PARSER_H_ */
