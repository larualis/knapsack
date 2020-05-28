//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_RESTRICTEDPRUNING_H
#define KNAPSACK_RESTRICTEDPRUNING_H


#include "Pruning.h"
#include "../revDP.h"

class RestrictedPruning  : public Pruning
{
public:
  RestrictedPruning(int numberOfElements, std::vector<std::vector<PruningSolution>>& thresholdValues, std::vector<int> functionsRestricted);
  
  bool shouldSolutionBeRemoved(static_vector& solution, int posOfOldSolution);
  
  void solutionWasAdded();
  
  void setupForNextElement();
  
  void updateNumberOfValidRounds_(int numberOfSolutionsToRemove);
  
private:
  std::vector<std::vector<PruningSolution>> thresholdValues_;
  
  std::vector<int> functionsRestricted_;
  
  int firstSolutionSatisfyingWeight_;
  
  int validRoundsOfCurrentSolution_;
  
  std::vector<int> numberOfValidRounds_;
  
  std::vector<int> prevNumberOfValidRounds_;
};


#endif //KNAPSACK_RESTRICTEDPRUNING_H
