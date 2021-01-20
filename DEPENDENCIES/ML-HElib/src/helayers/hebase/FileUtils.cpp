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

#include <sys/stat.h>

#include "FileUtils.h"
#include "boost/filesystem.hpp"
#include <filesystem>
#include <iomanip>
#include <iostream>

using namespace std;

namespace helayers {

// this method creates a new directory with name "dirName".
// it assumes a directory with the given name does not already exists
void FileUtils::makeDir(const string& dirName)
{

  // create an empty directory
  int err = mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  if (err == -1)
    throw runtime_error("Error creating directory " + dirName + " errno=" +
                        to_string(errno));
}

// this method creates a new directory with name "dirName" if it does not
// already exist.
// if such a directory already exists, it does nothing.
void FileUtils::createDir(const string& dirName)
{

  // check whether directory exists and if so, do nothing and return
  struct stat st;
  if (stat(dirName.c_str(), &st) == 0)
    return;

  // create an empty directory
  makeDir(dirName);
}

// this method creates a new directory with name "dirName".
// if such a directory already exists, is it being cleaned of its content.
void FileUtils::createCleanDir(const string& dirName)
{

  // make sure dir name is at least 3 chars long
  size_t dirLength = (dirName.find('/') == string::npos)
                         ? dirName.length()
                         : (dirName.length() - dirName.find('/') - 1);
  if (dirLength < 3)
    throw runtime_error("Cannot create a clean directory " + dirName +
                        " with " + to_string(dirLength) +
                        " characters, name must contain at least 3 characters");

  // check whether directory exists and if so, delete it alongside its content
  struct stat st;
  if (stat(dirName.c_str(), &st) == 0)
    boost::filesystem::remove_all(dirName);

  // create an empty directory
  makeDir(dirName);
}

long FileUtils::reportFileSize(const std::string& path,
                               const std::string& logicalName)
{
  std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
  auto sz = in.tellg();
  // auto sz=filesystem::file_size(path);
  string units[] = {"GB", "MB", "KB", ""};
  long unitSz[] = {1024 * 1024 * 1024, 1024 * 1024, 1024, 1};
  int i = 0;
  while (i < 3 && sz < unitSz[i])
    ++i;
  float fsz = 1.0 * sz / unitSz[i];

  string title = path;
  if (logicalName != "")
    title = logicalName;
  cout << "Size of " << title << ": " << fixed << setprecision(2) << fsz << " "
       << units[i] << endl;
  return sz;
}

bool FileUtils::fileExists(const std::string& filepath)
{
  return boost::filesystem::exists(filepath);
}
}
