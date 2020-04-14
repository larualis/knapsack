//
// Created by rick on 10.04.20.
//

#include <algorithm>
#include "ElementManager.h"

ElementManager::ElementManager(std::vector<std::vector<float>> rawElements, int numberOfElements, int numberOfFunctions):
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
    std::vector<std::vector<float>> rawElements(numberOfElements_, std::vector<float>(numberOfFunctions_, 0));
    
    for (auto &ele : elements_)
    {
      std::vector<float> rawElement;
      
      rawElement.push_back(ele.weight_);
      
      rawElement.insert(rawElement.end(), ele.values_.begin(), ele.values_.end());
      
      rawElements[ele.posOrderValueWeightRatio_[i] - 1] = rawElement;
    }
  
    orderedRawElementsValue_.emplace_back(rawElements);
  }
  
  //!generate orderedRawElementsPointer_
  updateOrderedRawElementsPointer_();
}


void ElementManager::makeSumOrder()
{
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (auto &ele: elements_)
  {
    orderVector.emplace_back(elementWithOrderValue(&ele, ele.SumOfposOrderValueWeightRatio_));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  std::vector<element> sortedElements;
  
  sortedElements.reserve(numberOfElements_);
  
  for(auto &ele: orderVector)
  {
    sortedElements.push_back(*ele.ele_);
  }
  
  elements_ = std::move(sortedElements);
  
  updateOrderedRawElementsPointer_();
}

void ElementManager::makeMaxOrder()
{
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (auto &ele: elements_)
  {
    float greatestPos = ele.posOrderValueWeightRatio_.front();
    
    for(auto pos: ele.posOrderValueWeightRatio_)
    {
      if(greatestPos < pos)
      {
        greatestPos = pos;
      }
    }
  
    greatestPos += 1 / ( numberOfElements_ * numberOfFunctions_) * ele.SumOfposOrderValueWeightRatio_;
    
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
  
  updateOrderedRawElementsPointer_();
}

void ElementManager::makeMinOrder()
{
  std::vector<elementWithOrderValue> orderVector;
  
  orderVector.reserve(numberOfElements_);
  
  for (auto &ele: elements_)
  {
    float smallestPos = ele.posOrderValueWeightRatio_.front();
    
    for(auto pos: ele.posOrderValueWeightRatio_)
    {
      if(smallestPos > pos)
      {
        smallestPos = pos;
      }
    }
  
    smallestPos += 1 / ( numberOfElements_ * numberOfFunctions_) * ele.SumOfposOrderValueWeightRatio_;
    
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
  
  updateOrderedRawElementsPointer_();
}

void ElementManager::updateOrderedRawElementsPointer_()
{
  orderedRawElementsPointer_.clear();
  
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    std::vector<element*> vec(numberOfElements_, nullptr);
    
    for (auto &ele : elements_)
    {
      vec[ele.posOrderValueWeightRatio_[i] - 1] = &ele;
    }
    
    orderedRawElementsPointer_.emplace_back(vec);
  }
}

void ElementManager::removeElementFromOrder(int elementNumber)
{
  std::vector<int> posOfElementToRemove = elements_[elementNumber - 1].posOrderValueWeightRatio_;
  
  for(int i = 0; i < numberOfFunctions_; ++i)
  {
    orderedRawElementsValue_[i].erase(orderedRawElementsValue_[i].begin() + posOfElementToRemove[i] - 1);
  }
  /*
  for(int i = 0; i < numberOfFunctions_; ++i)
  {
    orderedRawElementsPointer_[i].erase(orderedRawElementsPointer_[i].begin() + posOfElementToRemove[i] - 1);
  }
   */
  
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
  
  updateOrderedRawElementsPointer_();
}

std::vector<element> &ElementManager::getElements()
{
  return elements_;
}

const std::vector<std::vector<element *>> &ElementManager::getOrderedRawElementsPointer() const
{
  return orderedRawElementsPointer_;
}

const std::vector<std::vector<std::vector<float>>> &ElementManager::getOrderedRawElementsValue() const
{
  return orderedRawElementsValue_;
}

int ElementManager::getNumberOfElements() const
{
  return numberOfElements_;
}
