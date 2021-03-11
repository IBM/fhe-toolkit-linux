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

#ifndef SRC_HELAYERS_HELIBCONTEXT_H_
#define SRC_HELAYERS_HELIBCONTEXT_H_

#include "helayers/hebase/HeContext.h"
#include "helib/helib.h"
#include "HelibConfig.h"

namespace helayers {

///@brief An implementation of HeContext API with concrete subclasses for
/// HElib scheme.
class HelibContext : public HeContext
{
protected:
  HelibConfig config;

  helib::Context* context = NULL;
  helib::SecKey* secretKey = NULL;
  helib::PubKey* publicKey = NULL;

  long nslots = 0;

  bool mirrored = false;

public:
  HelibContext();
  virtual ~HelibContext();

  void init(const HeConfigRequirement& req) override;

  ///@brief Creates a new HelibContext for either CKKS or BGV, based on a preset
  /// configuration. See list of presets in HelibConfig.h
  ///
  ///@param preset Preset configuration
  static std::shared_ptr<HelibContext> create(HelibPreset preset);

  ///@brief Initalizes with a given preset. See list of presets in HelibConfig.h
  ///
  ///@param preset Preset configuration name
  void initPreset(HelibPreset preset);

  ///@brief Initialize context with given configuration.
  ///
  ///@param conf Configuration details
  virtual void init(const HelibConfig& conf);

  int getTopChainIndex() const override;

  inline int slotCount() const override { return nslots; }

  inline int getSecurityLevel() const override
  {
    return (int)(context->securityLevel());
  }

  inline bool hasSecretKey() const override { return secretKey != NULL; }

  inline const helib::PubKey& getPublicKey() const { return *publicKey; }
  inline const helib::SecKey& getSecretKey() const
  {
    if (!hasSecretKey())
      throw std::runtime_error("This context does not have a secret key");
    return *secretKey;
  }
  inline const helib::Context& getContext() const { return *context; }

  inline bool getMirrored() const { return mirrored; }
  inline void setMirrored(bool v) { mirrored = v; }

  void printSignature(std::ostream& out = std::cout) const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  void save(std::ostream& out, bool withSecretKey) override;

  void load(std::istream& in) override;

  void saveSecretKey(std::ostream& out) override;

  void loadSecretKey(std::istream& in) override;

  std::string getLibraryName() const override { return "HELIB"; }

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;
};
} // namespace helayers

#endif /* SRC_HELAYERS_HELIBCONTEXT_H_ */
