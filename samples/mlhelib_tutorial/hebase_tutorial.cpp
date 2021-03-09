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

#include <iostream>

#include "hebase_tutorial.h"

using namespace std;

int main(int argc, char** argv)
{

  string arg = "";
  if (argc > 1) {
    arg = argv[1];
  }

  if (arg == "basics")
    tut_1_basics();
  else if (arg == "plaintexts")
    tut_2_plaintexts();
  else if (arg == "io")
    tut_3_io();
  else if (arg == "integers")
    tut_4_integers();
  else {
    cout << "Usage: " << argv[0] << " <tutName>" << endl
         << "\t<tutName> can be:" << endl
         << "\t\tbasics" << endl
         << "\t\tplaintexts" << endl
         << "\t\tio" << endl
         << "\t\tintegers" << endl;
    exit(1);
  }
}