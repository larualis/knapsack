//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_UPPERBOUNDPRUNING_H
#define KNAPSACK_UPPERBOUNDPRUNING_H


#include "Pruning.h"
#include "../SolutionComparer.h"
#include "ParetoPruning.h"


class UpperBoundPruning : public Pruning
{
public:
  UpperBoundPruning(int numberOfElements, std::vector<int> functionsToCompare, int capacity, ElementManager& elementManager);
  
  bool shouldSolutionBeRemoved(static_vector& solution);
  
  void setupForNextElement(std::list<static_vector>& currentBestSolutions);

private:
  ElementManager elementManager_;
  
  std::vector<int> functionsToCompare_;
  
  int numberOfFunctions_;
  
  int capacity_;
  
  SolutionComparer solComp_;
  
  ParetoPruning paretoPruning_;
  
  std::list<static_vector> greedyCompletionOfCurrentBestSolution_;
  
  static_vector upperBound(static_vector &sol);
};


#endif //KNAPSACK_UPPERBOUNDPRUNING_H
