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

#ifndef SRC_HEDGE_HECONTEXT_H
#define SRC_HEDGE_HECONTEXT_H

#include <memory>
#include <iostream>
#include <string>
#include "HeTraits.h"

namespace hedge {

class AbstractCiphertext;
class AbstractPlaintext;
class AbstractEncoder;
class AbstractFunctionEvaluator;
class AbstractBitwiseEvaluator;

/// A main class representing an underlying HE library & scheme, configured,
/// initialized, and ready to start working.
///
/// This class bundles together all the data structures
/// required for performing HE operations, including the keys.
/// It either contains a public/private key pair, allowing all operations
/// including decryption, or just the public key allowing everything
/// but decryption.
///
/// It is used as input in constructor of many other classes
/// in this library. For example, to create a ciphertext using
/// the library represent by an HeContext, write:
/// CTile c(he);
///
/// It can be further used to query various properties of the underlying scheme
/// and library.
class HeContext
{
  double defaultScale = 1;

protected:
  HeTraits traits;

public:
  HeContext();

  virtual ~HeContext();

  HeContext(const HeContext& src) = delete;

  HeContext& operator=(const HeContext& src) = delete;

  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher() = 0;

  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain() = 0;

  virtual std::shared_ptr<AbstractEncoder> getEncoder() = 0;

  virtual std::shared_ptr<AbstractFunctionEvaluator> getFunctionEvaluator();

  virtual std::shared_ptr<AbstractBitwiseEvaluator> getBitwiseEvaluator()
  {
    throw std::runtime_error("This HEContext cannot provide BitwiseEvaluator");
    return nullptr;
  }

  /// Returns an HeTraits object containing various properties of the
  /// underlying scheme.
  inline const HeTraits& getTraits() { return traits; }

  /// Returns whether this context contains a secret key.
  /// If not, decryption and other operations relying on decryption
  /// will not be available (will throw an exception).
  virtual bool hasSecretKey() const = 0;

  /// The number of slots in each CTile (ciphertext) or PTile (plaintext)
  /// created over this context.
  virtual int slotCount() const = 0;

  virtual int getTopChainIndex() const = 0;

  /// Returns the security level supplied by this context.
  virtual int getSecurityLevel() const = 0;

  ///  Prints a summary of library details and configuration params.
  ///
  ///  @param[in] out output stream to write to
  virtual void printSignature(std::ostream& out = std::cout) const = 0;

  virtual void debugPrint(const std::string& title = "",
                          int verbose = 0,
                          std::ostream& out = std::cout) const = 0;

  ///  Saves this context to a stream in binary form.
  ///
  ///  @param[in] out output stream to write to
  ///  @param[in] withSecretKey whether to include the secret key
  virtual void save(std::ostream& out, bool withSecretKey);

  ///  Loads context saved by the save() method
  ///
  ///  @param[in] out input stream to read from
  virtual void load(std::istream& in);

  /// Save secret key to the given ostream. \n
  /// @param[out] ostream the binary stream to save to.
  ///
  /// @throw runtime_error if this HeContext doesn't have a secret key. i.e.
  ///        hasSecretKey() is false.
  virtual void saveSecretKey(std::ostream& out) = 0;

  /// Load secret key from the given istream. \n
  /// @param[in] istream the binary stream to load from.
  /// @throw runtime_error if this HeContext already has a secret key. i.e.
  ///        hasSecretKey() is ture.
  virtual void loadSecretKey(std::istream& in) = 0;

  ///  Saves this context to a file in binary form.
  ///
  ///  @param[in] fileName file to write to
  ///  @param[in] withSecretKey whether to include the secret key
  void saveToFile(const std::string& fileName, bool withSecretKey);

  ///  Loads context saved by the saveToFile() method
  ///
  ///  @param[in] fileName file to read from
  void loadFromFile(const std::string& fileName);

  /// save secret key to the given file. \n
  /// @param[out] fileName the path of the file to save to.
  /// @throw runtime_error if this HeContext doesn't have a secret key. i.e.
  ///        hasSecretKey() is false.
  void saveSecretKeyToFile(const std::string& fileName);

  /// load secret key from the given file. \n
  /// @param[in] fileName the path of the file to load from
  /// @throw runtime_error if this HeContext already has a secret key. i.e.
  ///        hasSecretKey() is ture.
  void loadSecretKeyFromFile(const std::string& fileName);

  inline double getDefaultScale() const { return defaultScale; }
  void setDefaultScale(double v) { defaultScale = v; }

  /// Returns the name of the underlying library
  virtual std::string getLibraryName() const = 0;

  /// Returns the name of the underlying scheme
  virtual std::string getSchemeName() const = 0;
};
}

#endif /* SRC_HEDGE_HECONTEXT_H */
