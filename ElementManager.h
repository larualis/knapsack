//
// Created by rick on 10.04.20.
//

#ifndef KNAPSACK_ELEMENTMANAGER_H
#define KNAPSACK_ELEMENTMANAGER_H

#include <vector>
#include <list>
#include <bits/unique_ptr.h>

struct element
{
  std::vector<float> values_;
  
  int weight_;
  
  std::vector<float> valueWeightRatios_;
  
  std::vector<int> posOrderValueWeightRatio_;
  
  int SumOfposOrderValueWeightRatio_;
  
  explicit element(std::vector<float>& rawElement):
      weight_(rawElement.front()),
      SumOfposOrderValueWeightRatio_(0),
      values_(rawElement.begin() + 1, rawElement.end())
  {
    posOrderValueWeightRatio_.resize(values_.size());
  
    valueWeightRatios_.reserve(values_.size());
    
    for( auto val: values_)
    {
      valueWeightRatios_.push_back(val / weight_);
    }
  }
};

struct elementWithOrderValue
{
  element* ele_;
  float valueForOrdering_;
  
  elementWithOrderValue( element* ele, float val):
      ele_(ele),
      valueForOrdering_(val)
  {}
  
  bool operator < (const elementWithOrderValue& str) const
  {
    return (valueForOrdering_ < str.valueForOrdering_);
  }
};

class ElementManager
{
private:
  std::vector<element> elements_;
  
  int numberOfElements_;
  
  int numberOfFunctions_;
  
  std::vector<std::vector<element*>> orderedRawElementsPointer_;
  
  std::vector<std::vector<std::vector<float>>> orderedRawElementsValue_;
  
public:
  ElementManager(std::vector<std::vector<float>> rawElements, int numberOfElements, int numberOfFunctions);
  
  ElementManager() {};
  
  void generateOrder();
  
  void removeElementFromOrder(int elementNumber);
  
  void reverseElements();
  
  void makeSumOrder();
  
  void makeMaxOrder();
  
  void makeMinOrder();
  
  const std::vector<std::vector<std::vector<float>>> &getOrderedRawElementsValue() const;
  
  const std::vector<std::vector<element *>> &getOrderedRawElementsPointer() const;
  
  std::vector<element> &getElements();
  
  int getNumberOfElements() const;

private:
  
  void updateOrderedRawElementsPointer_();
  
};


#endif //KNAPSACK_ELEMENTMANAGER_H
