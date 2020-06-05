//
// Created by rick on 04.06.20.
//

#ifndef KNAPSACK_APPROXRESTRICTEDDP_H
#define KNAPSACK_APPROXRESTRICTEDDP_H


#include "ApproxDP.h"
#include "../pruning_rules/RestrictedPruning.h"

class ApproxRestrictedDP
{
private:
  const Problem& problem_;
  
  ElementManager& elementManager_;
  
  int capacity_;
  
  int numberOfFunctions_;
  
  int numberOfCurrentElement_;
  
  std::vector<int> functionsToCompare_;
  
  std::vector<static_vector> solutions_;
  
  RemainingWeightPruning remainingWeightPruning_;
  
  ParetoPruning paretoPruner_;
  
  UpperBoundPruning upperBoundPruning_;
  
  RestrictedPruning restrictedPruning_;
  
  ApproxPruning approxPruning_;
  
  int frequency_;
  
  void errorRound(const Element& element,std::list<static_vector>& currentBestSolutions);
  
  void normalRound(const Element& element,std::list<static_vector>& currentBestSolutions);
  
  void handleNewSolution(static_vector &solution, int idxOldSolution);
  
  void handleNewSolutionWithError(static_vector &solution, bool &errorWasUsed, int idxOldSolution);
  
public:
  ApproxRestrictedDP(Problem& problem, std::vector<int> functionsToCompare, int frequency, std::vector<std::vector<PruningSolution>>& pruningValues);
  
  void run();
  
  std::vector<std::vector<int>> getNumberOfRemovedSolutionsPerRound();
  
  const std::vector<static_vector> &getSolutions() const;
};


#endif //KNAPSACK_APPROXRESTRICTEDDP_H
