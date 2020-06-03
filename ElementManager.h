//
// Created by rick on 10.04.20.
//

#ifndef KNAPSACK_ELEMENTMANAGER_H
#define KNAPSACK_ELEMENTMANAGER_H


#define NUMBEROFFUNCTIONS 2

#include <boost/container/static_vector.hpp>
using static_vector = boost::container::static_vector<int, NUMBEROFFUNCTIONS + 1>;


/**
#include <vector>
using static_vector = std::vector<int>;
 **/
#include <vector>
#include <list>

struct Element
{
  std::vector<int> values_;
  
  int weight_;
  
  std::vector<float> valueWeightRatios_;
  
  std::vector<int> posOrderValueWeightRatio_;
  
  int SumOfposOrderValueWeightRatio_;
  
  explicit Element(std::vector<int>& rawElement):
      weight_(rawElement.front()),
      SumOfposOrderValueWeightRatio_(0),
      values_(rawElement.begin() + 1, rawElement.end())
  {
    posOrderValueWeightRatio_.resize(values_.size());
  
    valueWeightRatios_.reserve(values_.size());
    
    for( auto val: values_)
    {
      valueWeightRatios_.push_back((float) val / (float) weight_);
    }
  }
};

struct ElementWithOrderValue
{
  Element* ele_;
  float valueForOrdering_;
  
  ElementWithOrderValue(Element* ele, float val):
      ele_(ele),
      valueForOrdering_(val)
  {}
  
  bool operator < (const ElementWithOrderValue& str) const
  {
    return (valueForOrdering_ < str.valueForOrdering_);
  }
};

class ElementManager
{
private:
  std::vector<Element> elements_;
  
  int numberOfElements_;
  
  int numberOfFunctions_;
  
  std::vector<std::vector<static_vector>> orderedRawElementsValue_;
  
public:
  ElementManager(std::vector<std::vector<int>> rawElements, int numberOfElements, int numberOfFunctions);
  
  ElementManager() {};
  
  void generateOrder();
  
  void removeElementFromOrder(int elementNumber);
  
  void reverseElements();
  
  void makeSumOrder(std::vector<int> functions);
  
  void makeMaxOrder(std::vector<int> functions);
  
  void makeMinOrder(std::vector<int> functions);
  
  const std::vector<std::vector<static_vector>> &getOrderedRawElementsValue() const;
  
  std::vector<Element> &getElements();
  
  int getNumberOfElements() const;
  
};


#endif //KNAPSACK_ELEMENTMANAGER_H
