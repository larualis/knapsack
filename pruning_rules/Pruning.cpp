//
// Created by rick on 27.05.20.
//

#include <numeric>
#include "Pruning.h"

Pruning::Pruning(int numberOfElements):
removedSolutionsPerRound_(numberOfElements, 0),
numberOfCurrentElement_(0)
{
}

int Pruning::removedSolutions()
{
  return std::accumulate(removedSolutionsPerRound_.begin(), removedSolutionsPerRound_.end(), 0);
}

void Pruning::updateRemovedSolutions(bool solutionWasRemoved)
{
  if(solutionWasRemoved)
  {
    ++removedSolutionsPerRound_[numberOfCurrentElement_];
  }
}
