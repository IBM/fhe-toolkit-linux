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

#ifndef SRC_HELAYERS_PTILE_H
#define SRC_HELAYERS_PTILE_H

#include "HeContext.h"
#include "impl/AbstractPlaintext.h"

namespace helayers {

/// A class representing an encoded (unencrypted) plaintext.
///
/// It's called a PTile because from a high-level point of view
/// we'll usually use several of these combined for holding a more complicated
/// object such as a matrix.
class PTile
{

  std::shared_ptr<AbstractPlaintext> impl;

  friend class CTile;
  friend class Encoder;

public:
  /// Constructs an empty object.
  /// @param[in] he the underlying context.
  PTile(HeContext& he);

  /// Copy constructor.
  /// @param[in] src Object to copy.
  PTile(const PTile& src);

  ~PTile();

  /// Copy from another object.
  /// @param[in] src Object to copy.
  PTile& operator=(const PTile& src);

  /// Reduces the chain-index property of this object by 1.
  /// Ignored if not supported.
  /// @throw runtime_error If chain index is already at lowest value
  void reduceChainIndex();

  /// Sets the chain-index property of this object to equal other object.
  /// Can only be used if other object's chain index is less than or equal to
  /// the chain index of this object.
  /// Ignored if not supported.
  /// @param[in] other object to get chain index from
  /// @throw runtime_error If chain index of other higher than this
  void setChainIndex(const PTile& other);

  /// Sets the chain-index property of this object to equal the specified value.
  /// Can only be used if the specified value is less than or equal to the chain
  /// index of this object.
  /// Ignored if not supported.
  /// @param[in] chainIndex The terget value to set the chain index to
  /// @throw runtime_error If the specified chain index is higher than this
  void setChainIndex(int chainIndex);

  /// Returns the value of the chain-index property of this object, which is a
  /// non-negative integer.
  /// Returns a negative value if not supported.
  int getChainIndex() const;

  /// A PTile represents a plaintext consisting of multuple slots.
  /// This method returns the number of slots in this object.
  int slotCount() const;

  ///  Saves this PTile to a file in binary form.
  ///
  ///  @param[in] fileName name of file to write to
  void saveToFile(const std::string& fileName) const;

  ///  Loads this PTile from a file saved by saveToFile()
  ///
  ///  @param[in] fileName name of file to read from
  void loadFromFile(const std::string& fileName);

  ///  Saves this PTile to a stream in binary form.
  ///
  ///  @param[in] stream output stream to write to
  std::streamoff save(std::ostream& stream) const;

  ///  Loads this PTile from a file saved by save()
  ///
  ///  @param[in] stream input stream to read from
  std::streamoff load(std::istream& stream);

  /// Prints information regarding this object for debug purposes.
  /// @param[in] title A title to be included in the printing.
  /// @param[in] maxElements The number of elements/slots to be included in the
  /// printing.
  /// @param[in] verbose Level of vebosity.
  /// @param[in] out An output stream to print the information into.
  void debugPrint(const std::string& title = "",
                  int maxElements = -1,
                  int verbose = 0,
                  std::ostream& out = std::cout) const;

  /// Reserved for debugging and internal use.
  const AbstractPlaintext& getImpl() const { return *impl; };
};
}

#endif /* SRC_HELAYERS_PTILE_H */
