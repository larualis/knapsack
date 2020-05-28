//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_REMAININGWEIGHTPRUNING_H
#define KNAPSACK_REMAININGWEIGHTPRUNING_H

#include <vector>
#include "Pruning.h"

class RemainingWeightPruning : public Pruning
{
public:
  RemainingWeightPruning(int numberOfElements, int sumOfWeights, int capacity);
  
  bool shouldSolutionBeRemoved(static_vector& solution);
  
  void setupForNextElement(int weight);
  
private:
  int remainingWeight_;
  
  int capacity_;
};


#endif //KNAPSACK_REMAININGWEIGHTPRUNING_H
