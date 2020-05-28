//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_PRUNING_H
#define KNAPSACK_PRUNING_H


#include <vector>
#include "../ElementManager.h"

class Pruning
{
protected:
  std::vector<int> removedSolutionsPerRound_;
  
  int numberOfCurrentElement_;
  
  void updateRemovedSolutions(bool solutionWasRemoved);
  
public:
  Pruning(int numberOfElements);
  
  int removedSolutions();
};


#endif //KNAPSACK_PRUNING_H
