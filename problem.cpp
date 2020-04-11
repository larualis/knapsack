//
// Created by rick on 13.03.20.
//

#include "problem.h"

#include <cmath>
#include <utility>
#include <cmath>

problem::problem(std::string filename, float capacity, int numberOfFunctions,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
                 capacity_(capacity),
                 numberOfFunctions_(numberOfFunctions),
                 restrictedFunctions_(std::move(restrictedFunctions)),
                 slack_(std::move(slack))
{
  initialiaze(filename);
}

problem::problem(std::string filename, int numberOfFunctions,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
    numberOfFunctions_(numberOfFunctions),
    restrictedFunctions_(std::move(restrictedFunctions)),
    slack_(std::move(slack))
{
  initialiaze(filename);
  
  capacity_ = std::floor(sumOfWeights_ /2);
}

void problem::initialiaze(std::string& filename)
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

void problem::readData(std::string& filename)
{
  std::ifstream file;
  
  file.open(filename);
  
  std::vector<float> element(numberOfFunctions_ + 1, 0);
  
  while (!file.eof())
  {
    for (int i = 0; i <= numberOfFunctions_; ++i)
    {
      file >> element[i];
    }
    elements_.push_back(element);
  }
  file.close();
}

void problem::orderInformation()
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

void problem::makeSumOrder()
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

void problem::makeMaxOrder()
{
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

void problem::makeMinOrder()
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

void problem::reverseElements()
{
  reverse(elements_.begin(),elements_.end());
  
  reverse(elementsWithInformation_.begin(), elementsWithInformation_.end());
}

const std::vector<std::vector<float>> &problem::getElements() const
{
  return elements_;
}

float problem::getCapacity() const
{
  return capacity_;
}

int problem::getNumberOfFunctions() const
{
  return numberOfFunctions_;
}

const std::vector<int> &problem::getRestrictedFunctions() const
{
  return restrictedFunctions_;
}

const std::vector<float> &problem::getSlack() const
{
  return slack_;
}

float problem::getSumOfWeights() const
{
  return sumOfWeights_;
}


const std::vector<elementsWithOrder> &problem::getElementsWithInformation() const
{
  return elementsWithInformation_;
}
