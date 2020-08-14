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

#ifndef SRC_ML_HE_EXAMPLES_HECONTEXT_H
#define SRC_ML_HE_EXAMPLES_HECONTEXT_H

#include <memory>
#include <string>

class AbstractCiphertext;
class AbstractPlaintext;
class AbstractEncoder;

class HeContext {

  double defaultScale=1;
public:

  HeContext();

  virtual ~HeContext();

  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher() = 0;

  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain() = 0;
  
  virtual std::shared_ptr<AbstractEncoder> getEncoder() = 0;

  virtual bool hasSecretKey() const = 0;

  virtual int slotCount() const = 0;

  virtual int getTopChainIndex() const = 0;

  virtual int getSecurityLevel() const = 0;

  virtual void printSignature(std::ostream&out) const =0;

  virtual void save(std::ostream&out,bool withSecretKey)=0;
  virtual void load(std::istream&out)=0;

  void saveToFile(const std::string& fileName,bool withSecretKey);
  void loadFromFile(const std::string& fileName);
  
  virtual bool automaticallyManagesChainIndices() const { return false; }

  inline double getDefaultScale() const { return defaultScale; }
  void setDefaultScale(double v) { defaultScale=v;}
};

#endif /* SRC_ML_HE_EXAMPLES_HECONTEXT_H */
