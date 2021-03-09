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

#ifndef SRC_HELAYERS_JSONSUBTREE_H
#define SRC_HELAYERS_JSONSUBTREE_H

#include <vector>
#include "JsonWrapper.h"

namespace helayers {

/// A class to represent a subtree (node and its children) of a JSON tree.
class JsonSubtree
{
  const boost::property_tree::ptree* subtree;

  /// Constructing new JsonSubtree given a pointer to Boost property tree. This
  /// is a private constructor accesible from within this class of from the
  /// JsonWrapper class in order to return its root as a subtree.
  ///  @param[in] subtree pointer to Boost property tree that's assumed to
  ///  remain valid in the lifetime of the JsonSubtree instance
  JsonSubtree(const boost::property_tree::ptree* subtree) : subtree(subtree) {}

  friend class JsonWrapper;

public:
  ~JsonSubtree() {}

  /// Get a subtree of a chile node
  /// @param[in] key key name or path delimited by .
  JsonSubtree getChild(const std::string& key) const;

  /// For a chile node that's an array, get an array of subtrees, each
  /// corresponds to an inner node in the child array
  /// @param[in] key key name or path delimited by .
  std::vector<JsonSubtree> getArrayChild(const std::string& key) const;

  /// For a child node that's an array, get an array with all the integer
  /// elements within the child array. If the array contains elements other than
  /// integers these elements will be ignored
  /// @param[in] key key name or path delimited by .
  std::vector<int> getIntArray(const std::string& key) const;

  /// For a child node that's an array, get an array with all the double
  /// elements within the child array. If the array contains elements other than
  /// doubles these elements will be ignored
  /// @param[in] key key name or path delimited by .
  std::vector<double> getDoubleArray(const std::string& key) const;

  /// Retrieves a string value by key name, or a path containing
  /// several keys delimited by .
  /// @param[in] key key name or path delimited by .
  std::string getString(const std::string& key = "") const;

  /// Retrieves an integer value by key name, or a path containing
  /// several keys delimited by .
  /// @param[in] key key name or path delimited by .
  int getInt(const std::string& key = "") const;

  /// Retrieves an double value by key name, or a path containing
  /// several keys delimited by .
  /// @param[in] key key name or path delimited by .
  double getDouble(const std::string& key = "") const;

  /// Returns an indication for whether a child node exists under the specified
  /// key
  /// @param[in] key key name or path delimited by .
  bool doesChildExist(const std::string& key) const;

  /// Returns an indication for whether a value exists under the specified key,
  /// that can be interpreted as a string
  /// @param[in] key key name or path delimited by .
  bool doesStringExist(const std::string& key = "") const;

  /// Returns an indication for whether a value exists under the specified key,
  /// that can be interpreted as an integer
  /// @param[in] key key name or path delimited by .
  bool doesIntExist(const std::string& key = "") const;

  /// Returns an indication for whether a value exists under the specified key,
  /// that can be interpreted as a double-typed number
  /// @param[in] key key name or path delimited by .
  bool doesDoubleExist(const std::string& key = "") const;
};
} // namespace helayers
#endif // SRC_HELAYERS_JSONSUBTREE_H
