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

#ifndef HELAYERS_CONFIG_H
#define HELAYERS_CONFIG_H

#include <string>
#include <cstdlib>

namespace helayers {

// Returns directory where examples are located.
// Default is "../examples"
// Can be overridden using environment variable HELAYERS_EXAMPLES_DIR
std::string getExamplesDir();

// Returns directory where large data sets are located.
// Default is "../examples/data"
// Can be overridden using environment variable HELAYERS_DATA_SETS_DIR
std::string getDataSetsDir();

// Returns directory where examples will write their output.
// (possibly clean it first).
// Default is "./output"
// Can be overridden using environment variable HELAYERS_EXAMPLES_OUTPUT_DIR
std::string getExamplesOutputDir();

// Returns directory where tests will write temp files.
// (possibly clean it first).
// Default is "./output"
// Can be overridden using environment variable HELAYERS_TESTS_OUTPUT_DIR
std::string getTestsOutputDir();

// Returns directory where resources to be used internally by the code are
// located. Default is "../resources" Can be overridden using environment
// variable HELAYERS_RESOURCES_DIR
std::string getResourcesDir();
} // namespace helayers
#endif