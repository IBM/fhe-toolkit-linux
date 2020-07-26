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

#ifndef STRAINER_LAYER_H_
#define STRAINER_LAYER_H_


#include "DoubleMatrixArray.h"
#include "h5Dumper.h"
#include "h5Parser.h"



class Layer {
  std::string name;
  int index;

public:
  static const std::string FLOW_TITLE;

  Layer() : index(0) {}

  virtual ~Layer(){}

  virtual void loadh5(int slot, const H5Parser& h5File) {}
  virtual void saveh5(int slot, H5Dumper& h5File) const {}

  virtual std::vector<DoubleMatrixArray> getAllWeightsPlain() const {
    return std::vector<DoubleMatrixArray>();
  }
  virtual void debugPrint(std::ostream& out, int verboseLevel = 0) const=0;

  inline void setName(const std::string& n) { name = n; }
  inline const std::string& getName() const { return name; }
  inline void setIndex(int i) { index = i; }
  inline int getIndex() const { return index; }

  virtual void clear()=0;
};

std::ostream& operator<<(std::ostream&out, const Layer&l);



class PlainLayer: public Layer {
public:
  PlainLayer() {
  }

  virtual ~PlainLayer();

  virtual DoubleMatrixArray forward(const DoubleMatrixArray& inVec)=0;

  virtual void debugPrint(std::ostream& out, int verboseLevel = 0) const;

  virtual void clear();

protected:
  virtual std::vector<DoubleMatrixArray*> getAllWeightsRefsPlain() {
    return std::vector<DoubleMatrixArray*>(0);
  }
};


class SquareActivationPlainLayer: public PlainLayer {

public:

  SquareActivationPlainLayer() {
  }
  
  virtual ~SquareActivationPlainLayer() {
  }

  virtual DoubleMatrixArray forward(const DoubleMatrixArray& inVec);

};


#endif /* STRAINER_LAYER_H_ */
