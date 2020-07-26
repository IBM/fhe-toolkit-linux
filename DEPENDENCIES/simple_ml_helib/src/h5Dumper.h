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

#ifndef STRAINER_H5DUMPER_H_
#define STRAINER_H5DUMPER_H_

#include <map>
#include <vector>

#include "H5Cpp.h"

class H5Dumper {
protected:
  std::string file_name;
  H5::H5File file;
public:

  H5Dumper(const std::string& f);

  virtual ~H5Dumper();

  void dumpBias(const std::vector<double>& bias, const std::string& layerName);

  void dumpFC(const std::vector<std::vector<double> >& weights,
      const std::string& layerName);

  void dumpFilters(
      const std::vector<std::vector<std::vector<double> > >& filters,
      const std::string& layerName);

  void dumpData(const std::vector<double>& data, const std::vector<int>& dims,
      const std::string& dataSetName);

  void dumpEmptyLayer(const std::string& layerName);

  void setLayerNames(const std::vector<std::string>& layerNames);

  void setLayerWeightNames(const std::string& layerName,
      const std::vector<std::string>& weightNames);

  bool objectExists(const std::string& name) const;

  inline void close() { file.close(); }

private:
  std::map<std::string, std::string> layerNamesToPathes;

  H5::DataSet createDataSet(H5::Group& group, const H5::DataSpace& dataSpace,
      const H5::DataType& dataType, const std::string& dataSetName);

  H5::DataSet createDataSet(const H5::DataSpace& dataSpace, const H5::DataType& dataType,
      const std::string& dataSetName);

  H5::Group createGroup(const std::string& groupName);

  H5::Group createSubGroup(H5::Group& parent, const std::string& groupName);
};

#endif /* STRAINER_H5DUMPER_H_ */
