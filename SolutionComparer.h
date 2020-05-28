//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_SOLUTIONCOMPARER_H
#define KNAPSACK_SOLUTIONCOMPARER_H


#include "ElementManager.h"

class SolutionComparer
{
public:
  SolutionComparer(int numberOfFunctions);
  
  bool dominates(static_vector& sol1, static_vector& sol2);
  
  bool dlex(static_vector& sol1, static_vector& sol2);
  
  bool lex(static_vector& sol1, static_vector& sol2);

private:
  int numberOfFunctions_;
};


#endif //KNAPSACK_SOLUTIONCOMPARER_H
