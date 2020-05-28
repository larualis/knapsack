//
// Created by rick on 28.05.20.
//

#ifndef KNAPSACK_RESTRICTEDDP_H
#define KNAPSACK_RESTRICTEDDP_H

#include <vector>
#include <list>
#include "../Problem.h"

#include "../pruning_rules/ParetoPruning.h"
#include "../pruning_rules/RemainingWeightPruning.h"
#include "../pruning_rules/RestrictedPruning.h"
#include "../pruning_rules/UpperBoundPruning.h"

class RestrictedDP
{
private:
  const Problem& problem_;
  
  ElementManager& elementManager_;
  
  int capacity_;
  
  int numberOfFunctions_;
  
  int numberOfCurrentElement_;
  
  std::vector<int> functionsToCompare_;
  
  SolutionComparer solComp;
  
  std::vector<static_vector> solutions_;
  
  RemainingWeightPruning remainingWeightPruning_;
  
  RestrictedPruning restrictedPruning_;
  
  ParetoPruning paretoPruner_;
  
  UpperBoundPruning upperBoundPruning_;

public:
  RestrictedDP(Problem& problem, std::vector<std::vector<PruningSolution>>& pruningValues);
  
  void run();
  
  void handleNewSolution(static_vector& solution, int idxOldSolution, std::list<static_vector>& currentBestSolutions);
  
  const std::vector<static_vector> &getSolutions() const;
  
};

#endif //KNAPSACK_RESTRICTEDDP_H
