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

#ifndef SRC_HELAYERS_HETRAITS_H_
#define SRC_HELAYERS_HETRAITS_H_

namespace helayers {

/// A set of flags characterizing the underyling HE scheme.
class HeTraits
{

  bool supportsBootstrapping = false;
  bool supportsExplicitRescale = false;
  bool automaticallyManagesChainIndices = false;
  bool automaticallyManagesRescale = false;
  bool supportsExplicitChainIndices = false;
  bool supportsComplexNumbers = false;
  bool supportsBitwiseOperations = false;
  bool isModularArithmetic = false;
  bool supportsScaledEncoding = false;
  unsigned long arithmeticModulus = 0;
  bool isDebugEmpty = false;
  bool supportsDecryptAddedNoise = false;

public:
  void intersect(const HeTraits& other);

  inline void setSupportsBootstrapping(bool val)
  {
    supportsBootstrapping = val;
  }
  inline void setSupportsExplicitRescale(bool val)
  {
    supportsExplicitRescale = val;
  }
  inline void setAutomaticallyManagesChainIndices(bool val)
  {
    automaticallyManagesChainIndices = val;
  }
  inline void setAutomaticallyManagesRescale(bool val)
  {
    automaticallyManagesRescale = val;
  }
  inline void setSupportsExplicitChainIndices(bool val)
  {
    supportsExplicitChainIndices = val;
  }
  inline void setSupportsComplexNumbers(bool val)
  {
    supportsComplexNumbers = val;
  }
  inline void setSupportsBitwiseOperations(bool val)
  {
    supportsBitwiseOperations = val;
  }
  inline void setIsModularArithmetic(bool val) { isModularArithmetic = val; }
  inline void setSupportsScaledEncoding(bool val)
  {
    supportsScaledEncoding = val;
  }
  inline void setArithmeticModulus(unsigned long val)
  {
    arithmeticModulus = val;
  }
  inline void setIsDebugEmpty(bool val) { isDebugEmpty = val; }
  inline void setSupportsDecryptAddedNoise(bool val)
  {
    supportsDecryptAddedNoise = val;
  }

  inline bool getSupportsBootstrapping() const { return supportsBootstrapping; }
  inline bool getSupportsExplicitRescale() const
  {
    return supportsExplicitRescale;
  }
  inline bool getAutomaticallyManagesChainIndices() const
  {
    return automaticallyManagesChainIndices;
  }
  inline bool getAutomaticallyManagesRescale() const
  {
    return automaticallyManagesRescale;
  }
  inline bool getSupportsExplicitChainIndices() const
  {
    return supportsExplicitChainIndices;
  }
  inline bool getSupportsComplexNumbers() const
  {
    return supportsComplexNumbers;
  }
  inline bool getSupportsBitwiseOperations() const
  {
    return supportsBitwiseOperations;
  }
  inline bool getIsModularArithmetic() const { return isModularArithmetic; }
  inline bool getSupportsScaledEncoding() const
  {
    return supportsScaledEncoding;
  }
  inline unsigned long getArithmeticModulus() const
  {
    return arithmeticModulus;
  }
  inline bool getIsDebugEmpty() const { return isDebugEmpty; }
  inline bool getSupportsDecryptAddedNoise() const
  {
    return supportsDecryptAddedNoise;
  }
};
}

#endif /* SRC_HELAYERS_HETRAITS_H_ */
