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
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
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

#include <vector>
#include "H5Cpp.h"

namespace hedge {

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
  H5Parser(std::string f);

  virtual ~H5Parser();

  void readData(const std::string& path,
                std::vector<double>& vals,
                std::vector<int>& dims);

  std::vector<double> parseBias(std::string path) const;

  std::vector<std::vector<double>> parseFC(std::string path) const;

  std::vector<std::vector<std::vector<double>>> parseFilters(
      std::string path) const;

  bool objectExists(const std::string& name) const;

  inline H5::H5File& getFile() { return file; }

  // bool sameHierarchy(const H5Parser& other) const;
};
}

#endif /* PARSER_H_ */
