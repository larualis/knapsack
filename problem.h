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
  
  std::vector<int> restrictedFunctions_;
  
  std::vector<float> slack_;
  
  std::vector<std::vector<float>> elements_;
public:
  float getCapacity() const;
  
  int getNumberOfFunctions() const;
  
  const std::vector<int> &getRestrictedFunctions() const;
  
  const std::vector<std::vector<float>> &getElements() const;

private:
  
  void readData(std::string& filename);

public:
  problem(std::string filename, float capacity, int numberOfFunctions,  std::vector<int> restrictedFunctions, std::vector<float> slack);
};


#endif //KNAPSACK_PROBLEM_H
