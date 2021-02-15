//
// Created by rick on 04.05.20.
//

#ifndef KNAPSACK_SOLUTION_H
#define KNAPSACK_SOLUTION_H


#include <vector>
#include "../Problem.h"

class Solution
{
private:
  
  Problem& problem_;
  
  double runtime_;
  
  std::vector<std::vector<int>> numberOfRemovedSolutionsPerRound_;
  
  std::vector<static_vector> solutions_;
  
public:
  
  Solution(Problem &problem);
  
  double getRuntime() const
  {
    return runtime_;
  }
  
  int getSolutionSize() const
  {
    return solutions_.size();
  }
  
  const std::vector<static_vector> &getSolutions() const
  {
    return solutions_;
  }
  
  const std::vector<std::vector<int>> &getNumberOfRemovedSolutionsPerRound() const
  {
    return numberOfRemovedSolutionsPerRound_;
  }
  
  void makeNormalSolution();
  
  void makeRestrictedSolution();
  
  void makeApproxSolution();
  
  void makeApproxRestrictedSolution();
};

#endif //KNAPSACK_SOLUTION_H
