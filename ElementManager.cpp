//
// Created by rick on 10.04.20.
//

#include <algorithm>
#include "ElementManager.h"

ElementManager::ElementManager(std::vector<std::vector<int>> rawElements, int numberOfElements, int numberOfFunctions):
numberOfElements_(numberOfElements),
numberOfFunctions_(numberOfFunctions)
{
  for (auto &rawEle: rawElements)
  {
    elements_.emplace_back(rawEle);
  }
  
  generateOrder();
}

void ElementManager::generateOrder()
{
  //!generate order and save it in each element
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    for (auto &ele: elements_)
    {
      orderVector.emplace_back(elementWithOrderValue(&ele, ele.valueWeightRatios_[i]));
    }
    
    std::sort(orderVector.rbegin(), orderVector.rend());
    
    for(int posInOrder = 0; posInOrder < numberOfElements_; ++posInOrder)
    {
      orderVector[posInOrder].ele_->posOrderValueWeightRatio_[i] = posInOrder + 1;
    }
    orderVector.clear();
  }
  
  for (auto &ele: elements_)
  {
    for(auto pos : ele.posOrderValueWeightRatio_)
    {
      ele.SumOfposOrderValueWeightRatio_ += pos;
    }
  }
  
  //!generate orderedRawElementsValue_
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    std::vector<static_vector> rawElements(numberOfElements_, static_vector(numberOfFunctions_, 0));
    
    for (auto &ele : elements_)
    {
      static_vector rawElement;
      
      rawElement.push_back(ele.weight_);
      
      rawElement.insert(rawElement.end(), ele.values_.begin(), ele.values_.end());
      
      rawElements[ele.posOrderValueWeightRatio_[i] - 1] = rawElement;
    }
  
    orderedRawElementsValue_.emplace_back(rawElements);
  }
}


void ElementManager::makeSumOrder(std::vector<int> functions)
{
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (auto &ele: elements_)
  {
    float sumOfOrder = 0;
  
    for(int i: functions)
    {
      sumOfOrder += ele.posOrderValueWeightRatio_[i - 1];
    }
    
    sumOfOrder += (float) ele.SumOfposOrderValueWeightRatio_ / (float)( numberOfElements_ * numberOfFunctions_);
    
    orderVector.emplace_back(elementWithOrderValue(&ele, sumOfOrder));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  std::vector<element> sortedElements;
  
  sortedElements.reserve(numberOfElements_);
  
  for(auto &ele: orderVector)
  {
    sortedElements.push_back(*ele.ele_);
  }
  
  elements_ = std::move(sortedElements);
}

void ElementManager::makeMaxOrder(std::vector<int> functions)
{
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (auto &ele: elements_)
  {
    float greatestPos = ele.posOrderValueWeightRatio_[functions.front() - 1];
    
    for(auto i : functions)
    {
      if(greatestPos < ele.posOrderValueWeightRatio_[i - 1])
      {
        greatestPos = ele.posOrderValueWeightRatio_[i - 1];
      }
    }
  
    greatestPos += (float) ele.SumOfposOrderValueWeightRatio_ / (float)( numberOfElements_ * numberOfFunctions_);
    
    orderVector.emplace_back(elementWithOrderValue(&ele, greatestPos));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  std::vector<element> sortedElements;
  
  sortedElements.reserve(numberOfElements_);
  
  for(auto &ele: orderVector)
  {
    sortedElements.push_back(*ele.ele_);
  }
  
  elements_ = std::move(sortedElements);
}

void ElementManager::makeMinOrder(std::vector<int> functions)
{
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (auto &ele: elements_)
  {
    float smallestPos = ele.posOrderValueWeightRatio_[functions.front() - 1];
  
    for(auto i : functions)
    {
      if(smallestPos > ele.posOrderValueWeightRatio_[i - 1])
      {
        smallestPos = ele.posOrderValueWeightRatio_[i - 1];
      }
    }
  
    smallestPos += (float) ele.SumOfposOrderValueWeightRatio_ / (float)( numberOfElements_ * numberOfFunctions_);
    
    orderVector.emplace_back(elementWithOrderValue(&ele, smallestPos));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  std::vector<element> sortedElements;
  
  sortedElements.reserve(numberOfElements_);
  
  for(auto &ele: orderVector)
  {
    sortedElements.push_back(*ele.ele_);
  }
  
  elements_ = std::move(sortedElements);
}

void ElementManager::removeElementFromOrder(int elementNumber)
{
  std::vector<int> posOfElementToRemove = elements_[elementNumber - 1].posOrderValueWeightRatio_;
  
  for(int i = 0; i < numberOfFunctions_; ++i)
  {
    orderedRawElementsValue_[i].erase(orderedRawElementsValue_[i].begin() + posOfElementToRemove[i] - 1);
  }
  
  for(auto ele = elements_.begin() + elementNumber; ele != elements_.end(); ++ele)
  {
    for (int i = 0; i < posOfElementToRemove.size(); ++i)
    {
      if(posOfElementToRemove[i] < ele->posOrderValueWeightRatio_[i])
      {
        --ele->posOrderValueWeightRatio_[i];
        
        --ele->SumOfposOrderValueWeightRatio_;
      }
    }
  }
}

void ElementManager::reverseElements()
{
  reverse(elements_.begin(),elements_.end());
}

std::vector<element> &ElementManager::getElements()
{
  return elements_;
}

const std::vector<std::vector<static_vector>> &ElementManager::getOrderedRawElementsValue() const
{
  return orderedRawElementsValue_;
}

int ElementManager::getNumberOfElements() const
{
  return numberOfElements_;
}
