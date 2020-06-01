//
// Created by rick on 27.05.20.
//

#include <numeric>
#include "Pruning.h"

Pruning::Pruning(int numberOfElements):
    numberOfRemovedSolutionsPerRound_(numberOfElements, 0),
    numberOfCurrentElement_(0)
{
}

int Pruning::numberOfRemovedSolutions()
{
  return std::accumulate(numberOfRemovedSolutionsPerRound_.begin(), numberOfRemovedSolutionsPerRound_.end(), 0);
}

void Pruning::updateRemovedSolutions(bool solutionWasRemoved)
{
  if(solutionWasRemoved)
  {
    ++numberOfRemovedSolutionsPerRound_[numberOfCurrentElement_ - 1];
  }
}

std::vector<int> Pruning::getNumberOfRemovedSolutionsPerRound()
{
  return numberOfRemovedSolutionsPerRound_;
}
