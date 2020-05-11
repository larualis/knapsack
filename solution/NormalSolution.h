//
// Created by rick on 04.05.20.
//

#ifndef KNAPSACK_NORMALSOLUTION_H
#define KNAPSACK_NORMALSOLUTION_H


#include "Solution.h"

class NormalSolution: public Solution
{
public:
  NormalSolution(Problem& problem): Solution(problem){};
  
  void generateSolutin() override;
};


#endif //KNAPSACK_NORMALSOLUTION_H
