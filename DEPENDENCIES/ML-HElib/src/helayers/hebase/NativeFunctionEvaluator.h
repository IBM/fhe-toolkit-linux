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

#ifndef SRC_HELAYERS_NATIVEFUNCTIONEVALUATOR_H
#define SRC_HELAYERS_NATIVEFUNCTIONEVALUATOR_H

#include "impl/AbstractFunctionEvaluator.h"
#include "HeContext.h"

namespace helayers {

/// Class for holding functions supplied directly by underlying schemes.
class NativeFunctionEvaluator
{

  std::shared_ptr<AbstractFunctionEvaluator> impl;

public:
  /// Constructs a ready to use object.
  /// @param[in] he the underlying context.
  NativeFunctionEvaluator(HeContext& he);

  ~NativeFunctionEvaluator();

  /// Copy constructor deleted. Construct new objects when needed.
  NativeFunctionEvaluator(const NativeFunctionEvaluator& src) = delete;

  /// Assignment deleted. Construct new objects when needed.
  NativeFunctionEvaluator& operator=(const NativeFunctionEvaluator& src) =
      delete;

  /// Raises every slot in given CTile to the power of p.
  /// @param[in/out] c CTile to work on
  /// @param[in] p power to raise by
  void powerInPlace(CTile& c, int p) const;

  /// Compute product of given vector of CTiles elementwise.
  /// Does so in a depth-efficient manner.
  /// @param[out] result CTile to store result
  /// @param[in] multiplicands vector of CTiles to multiply together.
  void totalProduct(CTile& result,
                    const std::vector<CTile>& multiplicands) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_NATIVEFUNCTIONEVALUATOR_H */
