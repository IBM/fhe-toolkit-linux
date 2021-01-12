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

//
// Defines always_assert(condition) which runs even in Release mode.
//

#ifndef SRC_HELAYERS_JSONWRAPPER_H
#define SRC_HELAYERS_JSONWRAPPER_H

#include <iostream>
#include <string>

#include <boost/property_tree/ptree_fwd.hpp>

namespace helayers {

class JsonSubtree;

/// A wrapper over a JSON tree and json I/O utils.
class JsonWrapper
{
  // Note, we keep a pointer here to avoid full include file
  boost::property_tree::ptree* pt = NULL;

  void assertInitialized() const;

public:
  ~JsonWrapper();

  /// Loads json data from given char buffer
  ///  @param[in] inBuf buffer to read from
  ///  @param[in] len length of buffer
  void load(const char* inBuf, int len);

  /// Loads json data from given string
  ///  @param[in] in string to read from
  void load(const std::string& in);

  /// Loads json data from given stream
  ///  @param[in] in stream to read from
  void load(std::istream& in);

  /// Loads json data from given stream in binary format
  /// @param[in] in stream to read from
  void loadFromBinary(std::istream& in);

  /// Write json data from given stream in binary format
  /// @param[in] out stream to write to
  void writeToBinary(std::ostream& out) const;

  /// Clears data (returns to uninitialized)
  void clear();

  /// Prints json to out
  /// @param[in] out stream to write to
  /// @param[in] pretty flag indicating whether to add indentation
  void print(std::ostream& out, bool pretty) const;

  /// Retrieves a string value by key name, or a path containing
  /// several keys delimited by .
  /// @param[in] key key name or path delimited by .
  std::string getString(const std::string& key) const;

  /// Retrieves an integer value by key name, or a path containing
  /// several keys delimited by .
  /// @param[in] key key name or path delimited by .
  int getInt(const std::string& key) const;

  /// Retrieves an double value by key name, or a path containing
  /// several keys delimited by .
  /// @param[in] key key name or path delimited by .
  double getDouble(const std::string& key) const;

  /// Returns a string containing this json data
  std::string toString() const;

  /// Returns true if this object is initialized (was loaded).
  bool isInitialized() const;

  /// Returns JsonSubtree object with the subtree being the root of the JSON
  /// structure.
  JsonSubtree getRoot() const;
};

std::ostream& operator<<(std::ostream& out, const JsonWrapper& jw);
}
#endif // SRC_HELAYERS_JSONWRAPPER_H
