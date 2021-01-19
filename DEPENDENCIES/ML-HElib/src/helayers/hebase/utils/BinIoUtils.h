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

#ifndef SRC_HELAYERS_BINIOUTILS_H
#define SRC_HELAYERS_BINIOUTILS_H

#include <iostream>
#include <string>

namespace helayers {

/// Utlities for IO operations on binary streams
class BinIoUtils
{

public:
  ///@brief Writes a string to a binary stream
  ///
  ///@param out Stream to write to
  ///@param str The string to write
  static void writeString(std::ostream& out, const std::string& str);

  ///@brief Writes a double to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The double to write
  static void writeDouble(std::ostream& out, double val);

  ///@brief Writes an int to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The int to write
  static void writeInt(std::ostream& out, int val);

  ///@brief Writes a size_t to a binary stream
  ///
  ///@param out Stream to write to
  ///@param val The size_t to write
  static void writeSizeT(std::ostream& out, size_t val);

  ///@brief Returns a string read from a binary stream
  ///
  ///@param in Stream to read from
  static std::string readString(std::istream& in);

  ///@brief Returns a double read from a binary stream
  ///
  ///@param in Stream to read from
  static double readDouble(std::istream& in);

  ///@brief Returns an int read from a binary stream
  ///
  ///@param in Stream to read from
  static int readInt(std::istream& in);

  ///@brief Returns a size_t read from a binary stream
  ///
  ///@param in Stream to read from
  static size_t readSizeT(std::istream& in);
};
}

#endif /* SRC_HELAYERS_BINIOUTILS_H */
