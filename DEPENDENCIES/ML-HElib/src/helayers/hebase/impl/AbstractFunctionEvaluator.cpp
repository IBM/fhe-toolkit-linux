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

#include <assert.h>
#include "AbstractFunctionEvaluator.h"
#include "helayers/hebase/Encoder.h"

using namespace std;

namespace helayers {

AbstractFunctionEvaluator::AbstractFunctionEvaluator(HeContext& he) : he(he) {}

AbstractFunctionEvaluator::~AbstractFunctionEvaluator() {}

void AbstractFunctionEvaluator::powerInPlace(AbstractCiphertext& cipher,
                                             int p) const
{
  throw runtime_error("not implemented");
}
void AbstractFunctionEvaluator::totalProduct(AbstractCiphertext& result, const std::vector<shared_ptr<helayers::AbstractCiphertext>>& multiplicands) const
{
    throw runtime_error("not implemented");
}
  
}
