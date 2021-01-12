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

#ifndef SRC_HELAYERS_HELIB_HELIBCONFIG_H_
#define SRC_HELAYERS_HELIB_HELIBCONFIG_H_

#include <iostream>

namespace helayers {

/// A class for holding HElib configuration parameters.
/// It doesn't yet cover all of HElib flags.
struct HelibConfig
{
  unsigned long m;
  unsigned long r;
  unsigned long L;
  unsigned long p = -1;

  ///   Number of columns of Key-Switching matix (default = 2 or 3)
  unsigned long c = 2;

  /// CKKS configuration with 8129 slots, low multiplication depth.
  /// NOTE: check if resulting security level suites your needs.
  static HelibConfig createCkks8192();

  /// CKKS configuration with 16384 slots and high multiplication depth.
  /// NOTE: check if resulting security level suites your needs.
  static HelibConfig createCkks16384();

  void load(std::istream& in);
  void save(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream& out, const HelibConfig& conf);
}

#endif /* SRC_HELAYERS_HELIB_HELIBCONFIG_H_ */
