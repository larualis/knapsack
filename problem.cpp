//
// Created by rick on 13.03.20.
//

#include "problem.h"

problem::problem(std::string filename, float capacity, int numberOfFunctions,
                 std::vector<std::pair<int, int>> restrictedFunctions):
                 capacity_(capacity),
                 numberOfFunctions_(numberOfFunctions),
                 restrictedFunctions_(restrictedFunctions)
{
  readData(filename);
}

void problem::readData(std::string filename)
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

const std::vector<std::pair<int, int>> &problem::getRestrictedFunctions() const
{
  return restrictedFunctions_;
}