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

//
// Defines always_assert(condition) which runs even in Release mode.
// Optionally, a message can be added: always_assert(condition, message): the
// message will be added to the assertion output when the condition isn't met.
//
// Example usage:
//
//   always_assert(dimensions == 3);
//
//   always_assert(dimensions == 3, "Cannot continue if the world is not 3D");
//

#ifndef SRC_HELAYERS_ALWAYSASSERT_H
#define SRC_HELAYERS_ALWAYSASSERT_H

#include <string>

namespace helayers {

void always_assert_fail(const char* conditionString,
                        const char* fileName,
                        unsigned int lineNum,
                        const char* funcName);

void always_assert_fail(const char* conditionString,
                        const char* fileName,
                        unsigned int lineNum,
                        const char* funcName,
                        const std::string& message);

#define always_assert(condition, ...)                                          \
  do {                                                                         \
    if (!(condition))                                                          \
      always_assert_fail(#condition,                                           \
                         __FILE__,                                             \
                         __LINE__,                                             \
                         __func__ __VA_OPT__(, ) __VA_ARGS__);                 \
  } while (0)
} // namespace helayers

#endif // SRC_HELAYERS_ALWAYSASSERT_H
