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

#include <vector>
#include "JsonSubtree.h"
#include <boost/property_tree/ptree.hpp>

using namespace std;

namespace helayers {

JsonSubtree JsonSubtree::getChild(const string& key) const
{
  if (!doesChildExist(key))
    throw runtime_error("No subtree exists under the specified key: " + key);

  return JsonSubtree(&subtree->get_child(key));
}

std::vector<JsonSubtree> JsonSubtree::getArrayChild(const string& key) const
{
  if (!doesChildExist(key))
    throw runtime_error("No subtree exists under the specified key: " + key);

  std::vector<JsonSubtree> res;
  for (auto const& item : subtree->get_child(key))
    res.push_back(JsonSubtree(&item.second));
  return res;
}

std::vector<int> JsonSubtree::getIntArray(const string& key) const
{
  std::vector<JsonSubtree> array = getArrayChild(key);

  std::vector<int> res;
  for (JsonSubtree item : array) {
    if (item.doesIntExist())
      res.push_back(item.getInt());
  }

  return res;
}

std::vector<double> JsonSubtree::getDoubleArray(const string& key) const
{
  std::vector<JsonSubtree> array = getArrayChild(key);

  std::vector<double> res;
  for (JsonSubtree item : array) {
    if (item.doesDoubleExist())
      res.push_back(item.getDouble());
  }

  return res;
}

string JsonSubtree::getString(const string& key) const
{
  if (!doesStringExist(key))
    throw runtime_error("No string exists under the specified key: " + key);

  return subtree->get<string>(key);
}

int JsonSubtree::getInt(const string& key) const
{
  if (!doesIntExist(key))
    throw runtime_error("No integer exists under the specified key: " + key);

  return subtree->get<int>(key);
}

double JsonSubtree::getDouble(const string& key) const
{
  if (!doesDoubleExist(key))
    throw runtime_error("No double exists under the specified key: " + key);

  return subtree->get<double>(key);
}

bool JsonSubtree::doesChildExist(const string& key) const
{
  return subtree->get_child_optional(key) ? true : false;
}

bool JsonSubtree::doesStringExist(const string& key) const
{
  try {
    subtree->get<string>(key);
    return true;
  } catch (...) {
    return false;
  }
}

bool JsonSubtree::doesIntExist(const string& key) const
{
  try {
    subtree->get<int>(key);
    return true;
  } catch (...) {
    return false;
  }
}

bool JsonSubtree::doesDoubleExist(const string& key) const
{
  try {
    subtree->get<double>(key);
    return true;
  } catch (...) {
    return false;
  }
}
} // namespace helayers