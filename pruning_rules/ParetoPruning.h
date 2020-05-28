//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_PARETOPRUNING_H
#define KNAPSACK_PARETOPRUNING_H


#include "Pruning.h"
#include "../SolutionComparer.h"

class ParetoPruning : public Pruning
{
public:
  ParetoPruning(int numberOfElements, int numberOfFunctions);
  
  bool shouldSolutionBeRemoved(static_vector& solution);
  
  void solutionWasAdded(static_vector& solution);
  
  void setupForNextElement(std::list<static_vector>* currentBestSolutions);

private:
  
  std::list<static_vector>* currentBestSolutions_;
  
  SolutionComparer solComp_;
  
  std::_List_iterator<static_vector> placeToAddSolution_;
};


#endif //KNAPSACK_PARETOPRUNING_H
