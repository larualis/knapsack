//
// Created by rick on 13.03.20.
//

#include "problem.h"

#include <utility>

problem::problem(std::string filename, float capacity, int numberOfFunctions,
                 std::vector<int> restrictedFunctions, std::vector<float> slack):
                 capacity_(capacity),
                 numberOfFunctions_(numberOfFunctions),
                 restrictedFunctions_(std::move(restrictedFunctions)),
                 slack_(std::move(slack))
{
  readData(filename);
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
