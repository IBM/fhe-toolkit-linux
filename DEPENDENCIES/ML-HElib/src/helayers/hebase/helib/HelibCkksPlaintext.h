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

#ifndef SRC_HELAYERS_HELIBCKKSPLAIN_H_
#define SRC_HELAYERS_HELIBCKKSPLAIN_H_

#include "helayers/hebase/impl/AbstractPlaintext.h"
#include "HelibCkksContext.h"

namespace helayers {

///@brief Concrete implementation of AbstractPlaintext API for HElib's CKKS
/// scheme.
class HelibCkksPlaintext : public AbstractPlaintext
{
  HelibCkksContext& heContext;

  helib::Ptxt<helib::CKKS> pt;

  /// @brief A default copy constructor.
  HelibCkksPlaintext(const HelibCkksPlaintext& src) = default;

  std::shared_ptr<AbstractPlaintext> doClone() const override;

  friend class HelibCkksEncoder;

public:
  /// @brief A constructor.
  HelibCkksPlaintext(HelibCkksContext& h)
      : AbstractPlaintext(h), heContext(h), pt(h.getContext())
  {}

  ~HelibCkksPlaintext() override{};

  /// @brief Copies this HelibCkksPlaintext and returns a pointer to the new
  /// copied object as shared_ptr<HelibCkksPlaintext>.
  std::shared_ptr<HelibCkksPlaintext> clone() const
  {
    return std::static_pointer_cast<HelibCkksPlaintext>(doClone());
  }

  /// @brief Saves this HelibCkksPlaintext to the receieved binary stream.
  /// @param stream The binary stream to save to.
  std::streamoff save(std::ostream& stream) const override;

  /// @brief Loads this HelibCkksPlaintext from the receieved binary stream.
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

  /// @brief Returns the internal plaintext object of the underlying Helib CKKS
  /// scheme.
  const helib::Ptxt<helib::CKKS>& getPlaintext() const;

  /// @brief Returns the internal plaintext object of the underlying Helib CKKS
  /// scheme.
  const helib::Ptxt<helib::CKKS>& getRaw() const { return pt; }
};
} // namespace helayers

#endif /* SRC_HELAYERS_HELIBCKKSPLAIN_H_ */
