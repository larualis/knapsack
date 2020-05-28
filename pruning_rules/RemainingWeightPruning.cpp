//
// Created by rick on 27.05.20.
//

#include "RemainingWeightPruning.h"

RemainingWeightPruning::RemainingWeightPruning(int numberOfElements, int sumOfWeights, int capacity):
    Pruning(numberOfElements),
    remainingWeight_(sumOfWeights),
    capacity_(capacity)
{
}

bool RemainingWeightPruning::shouldSolutionBeRemoved(static_vector &solution)
{
  bool rval = solution.front() + remainingWeight_ <= capacity_;
  
  updateRemovedSolutions(rval);
  
  return rval;
}

void RemainingWeightPruning::setupForNextElement(int weight)
{
  remainingWeight_ -= weight;
  
  ++numberOfCurrentElement_;
}
