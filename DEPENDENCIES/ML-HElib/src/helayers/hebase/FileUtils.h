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

#ifndef SRC_HELAYERS_FILEUTILS_H
#define SRC_HELAYERS_FILEUTILS_H

#include <deque>
#include <string>

namespace helayers {

/// @brief A class implementing methods to work with files.
class FileUtils
{

  FileUtils();

  /// @brief Creates a new directory with name "dirName".
  // This function assumes a directory with the given name does not already
  // exists
  static void makeDir(const std::string& dirName);

public:
  /// @brief Creates a directory with the given name, if it does not exist.
  /// If there is a directory with the given name already, does nothing.
  /// @param dirName The name of the directory to create.
  static void createDir(const std::string& dirName);

  /// @brief Creates a clean directory with the given name.
  /// If there is a directory with the given name already, all of its content is
  /// cleaned.
  /// @param dirName The name of the directory to create.
  /// @throw runtime_error If the length of "dirName" is less than 3.
  static void createCleanDir(const std::string& dirName);

  /// @brief Prints the size of the given directory.
  /// @param path        The relative path of the directory to report its
  /// size.
  /// @param logicalName The title used in the size report message. If
  /// empty,
  ///                        "path" is used instead.
  static long reportFileSize(const std::string& path,
                             const std::string& logicalName = "");

  /// @brief Returns true iff the file of the given path exists
  /// @param path        The relative path of the checked file
  static bool fileExists(const std::string& filepath);
};
}

#endif /* SRC_HELAYERS_FILEUTILS_H */
