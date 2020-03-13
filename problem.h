//
// Created by rick on 13.03.20.
//

#ifndef KNAPSACK_PROBLEM_H
#define KNAPSACK_PROBLEM_H

#include <fstream>
#include <vector>
#include <algorithm>

class problem
{
private:
  
  float capacity_;
  
  int numberOfFunctions_;
  
  std::vector<std::pair<int, int>> restrictedFunctions_;
public:
  float getCapacity() const;
  
  int getNumberOfFunctions() const;
  
  const std::vector<std::pair<int, int>> &getRestrictedFunctions() const;

private:
  
  std::vector<std::vector<float>> elements_;
public:
  const std::vector<std::vector<float>> &getElements() const;

private:
  
  void readData(std::string filename);

public:
  problem(std::string filename, float capacity, int numberOfFunctions,  std::vector<std::pair<int, int>> restrictedFunctions);
};


#endif //KNAPSACK_PROBLEM_H
