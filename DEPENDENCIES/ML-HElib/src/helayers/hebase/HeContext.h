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

#ifndef SRC_HELAYERS_HECONTEXT_H
#define SRC_HELAYERS_HECONTEXT_H

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "HeTraits.h"
#include "utils/JsonWrapper.h"

namespace helayers {

class AbstractCiphertext;
class AbstractPlaintext;
class AbstractEncoder;
class AbstractFunctionEvaluator;
class AbstractBitwiseEvaluator;

///@brief For internal use.
struct HeConfigRequirement
{
  int securityLevel;
  int integerPartPrecision;
  int fractionalPartPrecision;

  int numSlots = -1;
  int multiplicationDepth = -1;

  HeConfigRequirement(int securityLevel = 128,
                      int integerPartPrecision = 10,
                      int fractionalPartPrecision = 38)
      : securityLevel(securityLevel),
        integerPartPrecision(integerPartPrecision),
        fractionalPartPrecision(fractionalPartPrecision)
  {
  }
};

/// Use this macro to register a context class type for dynamic type loading.
/// Put this macro in the cpp file. The class must override the clone()
/// method for this to work.
/// It creates a static level variable, ensuring it will be initialized before
/// main.
#define REGISTER_HE_CONTEXT(heContextClass)                                    \
  static bool heContextClass##_static_entry =                                  \
      HeContext::internalRegisterContext(new heContextClass())

/// An abstract main class representing an underlying HE library & scheme,
/// configured, initialized, and ready to start working.
///
/// Upon initaliation, use on of the inheriting concrete classes such as
/// HelibCkksContext. Then continue with a generic reference to HeContext to
/// allow scheme obliviousness
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

  typedef std::map<std::string, const HeContext*> ContextMap;

  /// returns registered context map.
  /// Used instead of a static member to ensure it's already initialized when
  /// needed.
  static ContextMap& getRegisteredHeContextMap();

  /// Returns a string that identifies a concrete HeContext class for the
  /// purpose of dynamic loading.
  std::string getContextFileHeaderCode() const;

protected:
  HeTraits traits;

public:
  /// Constructs an empty object.
  HeContext();

  virtual ~HeContext();

  /// Copy constructor.
  /// @param[in] src Object to copy.
  HeContext(const HeContext& src) = delete;

  /// Copy from another object.
  /// @param[in] src Object to copy.
  HeContext& operator=(const HeContext& src) = delete;

  /// Internal use
  virtual void init(const HeConfigRequirement& req) = 0;

  /// Internal use
  virtual bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher() = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain() = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractEncoder> getEncoder() = 0;

  /// Do not use. Should be made private.
  virtual std::shared_ptr<AbstractFunctionEvaluator> getFunctionEvaluator();

  /// Do not use. Should be made private.
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

  /// Returns the highest available chain index (for schemes where it is
  /// applicable).
  virtual int getTopChainIndex() const = 0;

  /// Returns the security level supplied by this context.
  virtual int getSecurityLevel() const = 0;

  ///  Prints a summary of library details and configuration params.
  ///
  ///  @param[in] out output stream to write to
  virtual void printSignature(std::ostream& out = std::cout) const = 0;

  /// Prints detailed information for debug purposes.
  ///  @param[in] title Title to print along with the information.
  ///  @param[in] verbose Verbosity level
  ///  @param[in] out Output stream to print to
  virtual void debugPrint(const std::string& title = "",
                          int verbose = 0,
                          std::ostream& out = std::cout) const = 0;

  /// Returns the modulus chain bit length for each prime in the chain
  /// (where applicable).
  virtual std::shared_ptr<std::vector<uint64_t>> getModulusChain() const
  {
    throw std::runtime_error("getModulusChain() not implemented");
  };

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

  /// Returns default scale used in encoding (where applicable).
  virtual inline double getDefaultScale() const { return defaultScale; }

  /// Sets the default scale to be used in encoding (where applicable).
  ///  @param[in] v the default scale
  virtual void setDefaultScale(double v) { defaultScale = v; }

  /// Returns the name of the underlying library
  virtual std::string getLibraryName() const = 0;

  /// Returns the name of the underlying scheme
  virtual std::string getSchemeName() const = 0;

  /// Returns a signature of the context that distinguishes it enough to be able
  /// to load previously stored contexts based on their signatures.
  virtual std::string getSignature() const { return getSchemeName(); };

  /// For internal use
  inline virtual std::shared_ptr<JsonWrapper> getEstimatedLatencies() const
  {
    throw std::runtime_error("not implemented");
  };

  /// Returns a pointer to a context initialized from file.
  /// Context type is dynamically determined by content of file.
  /// @param[in] fileName file to read from
  static std::shared_ptr<HeContext> loadHeContextFromFile(
      const std::string& fileName);

  /// Returns a pointer to a context initialized from stream.
  /// Context type is dynamically determined by content of stream.
  /// @param[in] in stream to read from
  static std::shared_ptr<HeContext> loadHeContext(std::istream& in);

  /// Registers a context object for the purpose of dynamic loading.
  /// Don't call this directly. Use REGISTER_CONTEXT (see above)
  static bool internalRegisterContext(const HeContext* context);

  /// Returns an uninitialized context of the same type. Used for dynamic
  /// type loading among others.
  virtual std::shared_ptr<HeContext> clone() const;
};
}

#endif /* SRC_HELAYERS_HECONTEXT_H */
