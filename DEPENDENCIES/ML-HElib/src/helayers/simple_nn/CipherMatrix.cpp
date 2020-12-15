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

#include "CipherMatrix.h"

using namespace std;
using namespace boost::numeric::ublas;

namespace helayers {

CipherMatrix::CipherMatrix(HeContext& he) : he(&he), numFilledSlots(0) {}

CipherMatrix::~CipherMatrix() {}

streamoff CipherMatrix::save(ostream& stream) const
{
  HELAYERS_TIMER_SECTION("CipherMatrix::save");

  streampos streamStartPos = stream.tellp();

  size_t numRows = tiles.size(0);
  size_t numCols = tiles.size(1);

  stream.write(reinterpret_cast<const char*>(&numRows), sizeof(int));
  stream.write(reinterpret_cast<const char*>(&numCols), sizeof(int));
  stream.write(reinterpret_cast<const char*>(&numFilledSlots), sizeof(int));

  for (size_t i = 0; i < numRows; i++) {
    for (size_t j = 0; j < numCols; j++)
      tiles.at(i, j).save(stream);
  }

  streampos streamEndPos = stream.tellp();
  return streamEndPos - streamStartPos;
}

streamoff CipherMatrix::load(istream& stream)
{
  HELAYERS_TIMER_SECTION("CipherMatrix::load");

  streampos streamStartPos = stream.tellg();

  int numRows, numCols;

  stream.read(reinterpret_cast<char*>(&numRows), sizeof(int));
  stream.read(reinterpret_cast<char*>(&numCols), sizeof(int));
  stream.read(reinterpret_cast<char*>(&numFilledSlots), sizeof(int));

  basic_extents<size_t> extents(std::vector<size_t>{
      (long unsigned int)numRows, (long unsigned int)numCols});
  tiles.reshape(extents, CTile(*he));

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++)
      tiles.at(i, j).load(stream);
  }

  streampos streamEndPos = stream.tellg();

  return streamEndPos - streamStartPos;
}

void CipherMatrix::add(const CipherMatrix& other)
{
  HELAYERS_TIMER_SECTION("CipherMatrix::add");

  if (tiles.size(0) != other.tiles.size(0) ||
      tiles.size(1) != other.tiles.size(1) ||
      numFilledSlots != other.numFilledSlots)
    throw invalid_argument("Other has incompatible dimensions");

  for (size_t i = 0; i < tiles.size(); ++i)
    tiles[i].add(other.tiles[i]);
}

CipherMatrix CipherMatrix::getMatrixMultiply(const CipherMatrix& other) const
{
  HELAYERS_TIMER_SECTION("CipherMatrix::getMatrixMultiply");

  if (tiles.size(1) != other.tiles.size(0) ||
      numFilledSlots != other.numFilledSlots)
    throw invalid_argument("Other has incompatible dimensions");

  basic_extents<size_t> extents(
      std::vector<size_t>{tiles.size(0), other.tiles.size(1)});
  tensor<CTile> newTiles(extents, CTile(*he));

  for (size_t i = 0; i < newTiles.size(0); i++) {
    for (size_t j = 0; j < newTiles.size(1); j++) {
      for (size_t k = 0; k < tiles.size(1); k++) {
        CTile tmp(tiles.at(i, k));
        tmp.multiplyRaw(other.tiles.at(k, j));
        if (k == 0)
          newTiles.at(i, j) = tmp;
        else
          newTiles.at(i, j).add(tmp);
      }
    }
  }

  CipherMatrix res(*he);
  res.tiles = newTiles;
  res.numFilledSlots = numFilledSlots;
  res.relinearize();
  res.rescale();

  return res;
}

void CipherMatrix::square()
{
  HELAYERS_TIMER_SECTION("CipherMatrix::square");

  for (auto& tile : tiles)
    tile.square();
}

CipherMatrix CipherMatrix::getSquare() const
{
  CipherMatrix c(*this);
  c.square();
  return c;
}

void CipherMatrix::relinearize()
{
  HELAYERS_TIMER_SECTION("CipherMatrix::relinearize");

  for (auto& tile : tiles)
    tile.relinearize();
}

void CipherMatrix::rescale()
{
  HELAYERS_TIMER_SECTION("CipherMatrix::rescale");

  for (auto& tile : tiles)
    tile.rescale();
}

int CipherMatrix::getChainIndex() const
{
  if (tiles.size() == 0)
    throw runtime_error("Cipher matrix has not been encoded yet");

  return tiles(0).getChainIndex();
}
}
