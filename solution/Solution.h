//
// Created by rick on 04.05.20.
//

#ifndef KNAPSACK_SOLUTION_H
#define KNAPSACK_SOLUTION_H


#include <vector>
#include "../Problem.h"

class Solution
{
protected:
  
  Problem& problem_;
  
  int runtime_;
  
  std::vector<std::vector<int>> numberOfRemovedSolutionsPerRound_;
  
protected:
  
  std::vector<static_vector> solutions_;
  
public:
  
  Solution(Problem &problem):
      problem_(problem),
      runtime_(0)
  {
  }
  
  int getRuntime() const
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
  
  virtual void generateSolutin(){};
  
};

#endif //KNAPSACK_SOLUTION_H
