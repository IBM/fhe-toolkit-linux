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

#ifndef SRC_HELAYERS_HELIBBGVPLAIN_H_
#define SRC_HELAYERS_HELIBBGVPLAIN_H_

#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "HelibBgvContext.h"

namespace helayers {

///@brief Concrete implementation of AbstractPlaintext API for HElib's BGV
/// scheme.
class HelibBgvPlaintext : public AbstractPlaintext
{

  HelibBgvContext& heContext;

  helib::Ptxt<helib::BGV> pt;

  /// @brief A default copy constructor.
  HelibBgvPlaintext(const HelibBgvPlaintext& src) = default;

  /// @brief Copies this HelibBgvPlaintext and returns a pointer to the new
  /// copied object as shared_ptr<AbstractPlaintext>.
  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class HelibBgvEncoder;

public:
  /// @brief A constructor.
  HelibBgvPlaintext(HelibBgvContext& h)
      : AbstractPlaintext(h), heContext(h), pt(h.getContext())
  {
  }

  ~HelibBgvPlaintext() override{};

  /// @brief Copies this HelibBgvPlaintext and returns a pointer to the new
  /// copied object as shared_ptr<HelibBgvPlaintext>.
  std::shared_ptr<HelibBgvPlaintext> clone() const
  {
    return std::static_pointer_cast<HelibBgvPlaintext>(doClone());
  }

  /// @brief Saves this HelibBgvPlaintext to the receieved binary stream.
  /// @param stream The binary stream to save to.
  std::streamoff save(std::ostream& stream) const override;

  /// @brief Loads this HelibBgvPlaintext from the receieved binary stream.
  /// @param stream The binary stream to load from.
  std::streamoff load(std::istream& stream) override;

  /// @brief This function has no effect in this implementation of plaintext.
  void reduceChainIndex() override;

  /// @brief This function has no effect in this implementation of plaintext.
  void setChainIndex(const AbstractPlaintext& other) override;

  /// @brief This function has no effect in this implementation of plaintext.
  void setChainIndex(int chainIndex) override;

  /// @brief Returns an arbitrary value.
  int getChainIndex() const override;

  /// @brief Returns the number of slots of this plaintext.
  int slotCount() const override;

  /// @brief Returns the internal plaintext object of the underlying Helib BGV
  /// scheme.
  const helib::Ptxt<helib::BGV>& getPlaintext() const;

  /// @brief Writes the given helib::Ptxt into the given binary stream.
  /// @param out The binary stream to save to.
  /// @param pt  The helib::Ptxt object to save.
  static void writePtxtToBinary(std::ostream& out,
                                const helib::Ptxt<helib::BGV>& pt);

  /// @brief Reads the given helib::Ptxt object from the given binary stream.
  /// @param in       The binary stream to load from.
  /// @param pt       The helib::Ptxt object to read.
  /// @param context  The inner helib::Context object HElib.
  static void readPtxtFromBinary(std::istream& in,
                                 helib::Ptxt<helib::BGV>& pt,
                                 const helib::Context& context);
};
}

#endif /* SRC_HELAYERS_HELIBBGVPLAIN_H_ */
