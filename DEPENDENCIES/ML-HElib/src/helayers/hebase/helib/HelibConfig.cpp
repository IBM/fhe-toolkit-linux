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

#include "HelibConfig.h"

namespace helayers {

void HelibConfig::initPreset(HelibPreset preset)
{
  p = -1;
  c = 2;
  m = 0;
  L = 0;
  switch (preset) {
  case HELIB_NOT_SECURE_CKKS_512_FAST:
    m = 512 * 4;
    r = 52;
    L = 1024;
    break;

  case HELIB_CKKS_8192:
    m = 8192 * 4;
    r = 40;
    L = 200;
    break;
  case HELIB_CKKS_16384:
    m = 16384 * 4;
    r = 50;
    L = 700;
    break;

  case HELIB_CKKS_32768:
    m = 32768 * 4;
    r = 50;
    L = 700;
    break;

  case HELIB_NOT_SECURE_BGV_24:
    p = 4999;
    r = 1; // Hensel lifting
    m = 32109;
    L = 500;
    break;
  default:
    throw std::invalid_argument("Unknown preset");
  }
}

void HelibConfig::load(std::istream& in)
{
  in.read((char*)&m, sizeof(m));
  in.read((char*)&r, sizeof(r));
  in.read((char*)&L, sizeof(L));
  in.read((char*)&c, sizeof(c));
}

void HelibConfig::save(std::ostream& out) const
{
  out.write((char*)&m, sizeof(m));
  out.write((char*)&r, sizeof(r));
  out.write((char*)&L, sizeof(L));
  out.write((char*)&c, sizeof(c));
}

std::ostream& operator<<(std::ostream& out, const HelibConfig& conf)
{
  out << "m=" << conf.m << " r=" << conf.r << " L=" << conf.L
      << " c=" << conf.c;
  return out;
}
}
