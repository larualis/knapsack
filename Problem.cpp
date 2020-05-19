//
// Created by rick on 13.03.20.
//

#include "Problem.h"

#include <cmath>
#include <utility>
#include <sstream>

Problem::Problem(std::string filename, float capacity,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
                 capacity_(capacity),
                 restrictedFunctions_(std::move(restrictedFunctions)),
                 slack_(std::move(slack))
{
  std::vector<std::vector<int>> rawElements;
  
  readData(filename, rawElements);
  
  sumOfWeights_ = 0;
  
  for (auto &ele: rawElements)
  {
    sumOfWeights_ += ele[0];
  }
  
  numberOfFunctions_ = rawElements.front().size() - 1;
  
  eleManager_ = ElementManager(rawElements, rawElements.size(), numberOfFunctions_);
}

Problem::Problem(std::string filename,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
    restrictedFunctions_(std::move(restrictedFunctions)),
    slack_(std::move(slack))
{
  std::vector<std::vector<int>> rawElements;
  
  readData(filename, rawElements);
  
  sumOfWeights_ = 0;
  
  for (auto &ele: rawElements)
  {
    sumOfWeights_ += ele[0];
  }
  
  numberOfFunctions_ = rawElements.front().size() - 1;
  
  capacity_ = std::floor(sumOfWeights_ /2);
  
  eleManager_ = ElementManager(rawElements, rawElements.size(), numberOfFunctions_);
  
  slack_ = std::vector<float>(restrictedFunctions_.size(), 0);
  
  int k = 0;
  for(auto i : restrictedFunctions_ )
  {
    for (auto& ele: eleManager_.getElements())
    {
      slack_[k] += ele.values_[i - 1];
    }
    ++k;
  }
  
  for(auto &val : slack_)
  {
    //!this should be average value * 10% of number of Elements, should also include number of restricted functions
    val = (val / (float) eleManager_.getNumberOfElements()) * (float) eleManager_.getNumberOfElements() * 0.6 * 0.05;
  }
}

void Problem::readData(std::string& filename, std::vector<std::vector<int>>& rawElements)
{
  std::ifstream file;
  
  file.open(filename);
  
  std::string line;
  
  while(std::getline(file, line))
  {
    std::vector<int> element;
    
    std::stringstream ss(line);
    
    float val;
    
    while(ss >> val)
    {
      element.push_back(val);
    }
    
    rawElements.push_back(element);
  }
  
  file.close();
}


void Problem::makeSumOrder(std::vector<int> functionsForOrder )
{
  if(functionsForOrder.empty())
  {
    for(int i = 1; i <= numberOfFunctions_; ++i)
    {
      functionsForOrder.push_back(i);
    }
  }

  eleManager_.makeSumOrder(functionsForOrder);
}

void Problem::makeMaxOrder(std::vector<int> functionsForOrder )
{
  if(functionsForOrder.empty())
  {
    for(int i = 1; i <= numberOfFunctions_; ++i)
    {
      functionsForOrder.push_back(i);
    }
  }
  
  eleManager_.makeMaxOrder(functionsForOrder);
}

void Problem::makeMinOrder(std::vector<int> functionsForOrder )
{
  if(functionsForOrder.empty())
  {
    for(int i = 1; i <= numberOfFunctions_; ++i)
    {
      functionsForOrder.push_back(i);
    }
  }
  
  eleManager_.makeMinOrder(functionsForOrder);
}

void Problem::reverseElements()
{
  eleManager_.reverseElements();
}

int Problem::getCapacity() const
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

int Problem::getSumOfWeights() const
{
  return sumOfWeights_;
}

ElementManager &Problem::getEleManager()
{
  return eleManager_;
}
