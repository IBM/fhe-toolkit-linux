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

#ifndef SRC_HELAYERS_SAVEABLE_H
#define SRC_HELAYERS_SAVEABLE_H

#include <iostream>
#include <fstream>
#include <string>

class Saveable
{
public:
  static std::ofstream openOfstream(const std::string& fileName);
  static std::ifstream openIfstream(const std::string& fileName);

  ///  Saves this Saveable object to a file in binary form.
  ///
  ///  @param[in] fileName name of file to write to
  std::streamoff saveToFile(const std::string& fileName) const;

  ///  Loads this Saveable object from a file saved by saveToFile()
  ///
  ///  @param[in] fileName name of file to read from
  std::streamoff loadFromFile(const std::string& fileName);

  ///  Saves this Saveable object to a stream in binary form.
  ///
  ///  @param[in] stream output stream to write to
  virtual std::streamoff save(std::ostream& stream) const = 0;

  ///  Loads this Saveable object from a file saved by save()
  ///
  ///  @param[in] stream input stream to read from
  virtual std::streamoff load(std::istream& stream) = 0;
};

#endif