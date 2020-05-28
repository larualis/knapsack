//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_NORMALDP_H
#define KNAPSACK_NORMALDP_H

#include <vector>
#include <list>
#include "../Problem.h"

#include "../pruning_rules/ParetoPruning.h"
#include "../pruning_rules/RemainingWeightPruning.h"
#include "../pruning_rules/UpperBoundPruning.h"

class NormalDP
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
  
  ParetoPruning paretoPruner_;
  
  UpperBoundPruning upperBoundPruning_;

public:
  NormalDP(Problem& problem, std::vector<int> functionsToCompare);
  
  void run();
  
  void handleNewSolution(static_vector& solution, std::list<static_vector>& currentBestSolutions);
  
  const std::vector<static_vector> &getSolutions() const;
  
};


#endif //KNAPSACK_NORMALDP_H
