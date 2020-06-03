//
// Created by rick on 01.06.20.
//

#ifndef KNAPSACK_APPROXDP_H
#define KNAPSACK_APPROXDP_H


#include <vector>
#include <list>
#include "../Problem.h"
#include "../pruning_rules/RemainingWeightPruning.h"
#include "../pruning_rules/ParetoPruning.h"
#include "../pruning_rules/UpperBoundPruning.h"
#include "../pruning_rules/ApproxPruning.h"

class ApproxDP
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
  
  ApproxPruning approxPruning_;
  
  int frequency_;

public:
  ApproxDP(Problem& problem, std::vector<int> functionsToCompare, int frequency);
  
  void run();
  
  void errorRound(const Element& element,std::list<static_vector>& currentBestSolutions);
  
  void normalRound(const Element& element,std::list<static_vector>& currentBestSolutions);
  
  void handleNewSolution(static_vector &solution);
  
  void handleNewSolutionWithError(static_vector& solution, std::list<static_vector>& currentBestSolutions, bool& errorWasUsed, int& lastSolutionWeight);
  
  std::vector<std::vector<int>> getNumberOfRemovedSolutionsPerRound();
  
  const std::vector<static_vector> &getSolutions() const;
  
};


#endif //KNAPSACK_APPROXDP_H
