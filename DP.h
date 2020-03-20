//
// Created by rick on 11.03.20.
//

#ifndef KNAPSACK_DP_H
#define KNAPSACK_DP_H


#include <vector>
#include "problem.h"

class DP
{
private:
  const std::vector<std::vector<float>>& elements_;
 
  float capacity_;
 
  int numberOfFunctions_;
  
  std::vector<int> functionSubset_;
  
  std::vector<std::vector<float>> solutions_;
  
public:
  DP(const problem& Problem, std::vector<int>& functionsToCompare);
  
  void run();
  
  const std::vector<std::vector<float>> &getSolutions() const;

private:
  
  bool dominates(std::vector<float>& sol1, std::vector<float>& sol2, bool lastElement);
  
};


#endif //KNAPSACK_DP_H
