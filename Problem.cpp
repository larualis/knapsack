//
// Created by rick on 13.03.20.
//

#include "Problem.h"

#include <cmath>
#include <utility>
#include <sstream>


Problem::Problem(std::string filename,
                 std::vector<int> restrictedFunctions, std::vector<float> slack, double error):
    restrictedFunctions_(std::move(restrictedFunctions)),
    slack_(std::move(slack)),
    error_(error)
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

int Problem::getNumberOfElements()
{
  return eleManager_.getNumberOfElements();
}

std::vector<int> Problem::getVectorOfAllFunctions_()
{
  std::vector<int> functionsToCompare(numberOfFunctions_, 0);
  
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    functionsToCompare[i] = i + 1;
  }
  
  return functionsToCompare;
}

double Problem::getError() const
{
  return error_;
}
