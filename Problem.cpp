//
// Created by rick on 13.03.20.
//

#include "Problem.h"

#include <cmath>
#include <utility>
#include <cmath>
#include <sstream>

Problem::Problem(std::string filename, float capacity, int numberOfFunctions,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
                 capacity_(capacity),
                 restrictedFunctions_(std::move(restrictedFunctions)),
                 slack_(std::move(slack))
{
  initialiaze(filename);
  
  eleManager_ = ElementManager(elements_, elements_.size(), numberOfFunctions_);
}

Problem::Problem(std::string filename, int numberOfFunctions,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
    restrictedFunctions_(std::move(restrictedFunctions)),
    slack_(std::move(slack))
{
  initialiaze(filename);
  
  capacity_ = std::floor(sumOfWeights_ /2);
  
  eleManager_ = ElementManager(elements_, elements_.size(), numberOfFunctions_);
}

void Problem::initialiaze(std::string& filename)
{
  readData(filename);
  
  sumOfWeights_ = 0;
  for (auto &ele: elements_)
  {
    sumOfWeights_ += ele[0];
  
    elementsWithInformation_.emplace_back(elementsWithOrder(ele));
  }
  
  orderInformation();
}

void Problem::readData(std::string& filename)
{
  std::ifstream file;
  
  file.open(filename);
  
  std::string line;
  
  while(std::getline(file, line))
  {
    std::vector<float> element;
    
    std::stringstream ss(line);
    
    float val;
    
    while(ss >> val)
    {
      element.push_back(val);
    }
    
    elements_.push_back(element);
  }
  
  numberOfFunctions_ = elements_.front().size() - 1;
  
  file.close();
}

void Problem::orderInformation()
{
  std::vector<elementWithValue> orderVector;
  
  orderVector.reserve(elements_.size());
  
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    for (auto &ele: elementsWithInformation_)
    {
      orderVector.emplace_back(elementWithValue(&ele, ele.valueForOrder_[i]));
    }
    
    std::sort(orderVector.rbegin(), orderVector.rend());
    
    for( int j = 0; j < orderVector.size(); ++j)
    {
      orderVector[j].element->posAccordingToOrder_[i] = j + 1;
    }
    orderVector.clear();
  }
}

void Problem::makeSumOrder()
{
  std::vector<elementWithValue> orderVector;
  
  orderVector.reserve(elements_.size());
  
  elements_.clear();
  
  for (auto &ele: elementsWithInformation_)
  {
    orderVector.emplace_back(elementWithValue(&ele, ele.sumOfPosOrder()));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  std::vector<elementsWithOrder> sortedElementsWithOrder;
  
  for(auto &ele: orderVector)
  {
    elements_.push_back(ele.element->element_);
    
    sortedElementsWithOrder.push_back(*ele.element);
  }
  
  elementsWithInformation_ = std::move(sortedElementsWithOrder);
}

void Problem::makeMaxOrder()
{
  eleManager_.makeMaxOrder();
  
  std::vector<elementWithValue> orderVector;
  
  orderVector.reserve(elements_.size());
  
  for (auto &ele: elementsWithInformation_)
  {
    float val = ele.posAccordingToOrder_.front();
    
    for(auto posVal: ele.posAccordingToOrder_)
    {
      if(val < posVal)
      {
        val = posVal;
      }
    }
    
    val += 1/( elements_.size() * numberOfFunctions_) * ele.sumOfPosOrder();
    
    orderVector.emplace_back(elementWithValue(&ele, val));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  elements_.clear();
  
  std::vector<elementsWithOrder> sortedElementsWithOrder;
  
  for(auto &ele: orderVector)
  {
    elements_.push_back(ele.element->element_);
    
    sortedElementsWithOrder.push_back(*ele.element);
  }
  
  elementsWithInformation_ = std::move(sortedElementsWithOrder);
}

void Problem::makeMinOrder()
{
  std::vector<elementWithValue> orderVector;
  
  orderVector.reserve(elements_.size());
  
  for (auto &ele: elementsWithInformation_)
  {
    float val = ele.posAccordingToOrder_.front();
    
    for(auto posVal: ele.posAccordingToOrder_)
    {
      if(val > posVal)
      {
        val = posVal;
      }
    }
    
    val += 1/( elements_.size() * numberOfFunctions_) * ele.sumOfPosOrder();
    
    orderVector.emplace_back(elementWithValue(&ele, val));
  }
  
  std::sort(orderVector.begin(), orderVector.end());
  
  elements_.clear();
  
  std::vector<elementsWithOrder> sortedElementsWithOrder;
  
  for(auto &ele: orderVector)
  {
    elements_.push_back(ele.element->element_);
    
    sortedElementsWithOrder.push_back(*ele.element);
  }
  
  elementsWithInformation_ = std::move(sortedElementsWithOrder);
}

void Problem::reverseElements()
{
  reverse(elements_.begin(),elements_.end());
  
  reverse(elementsWithInformation_.begin(), elementsWithInformation_.end());
  
  eleManager_.reverseElements();
}

const std::vector<std::vector<float>> &Problem::getElements() const
{
  return elements_;
}

float Problem::getCapacity() const
{
  return capacity_;
}

int Problem::getNumberOfFunctions() const
{
  return numberOfFunctions_;
}

const std::vector<int> &Problem::getRestrictedFunctions() const
{
  return restrictedFunctions_;
}

const std::vector<float> &Problem::getSlack() const
{
  return slack_;
}

float Problem::getSumOfWeights() const
{
  return sumOfWeights_;
}

const std::vector<elementsWithOrder> &Problem::getElementsWithInformation() const
{
  return elementsWithInformation_;
}

const ElementManager &Problem::getEleManager() const
{
  return eleManager_;
}
