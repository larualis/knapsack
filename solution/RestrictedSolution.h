//
// Created by rick on 04.05.20.
//

#ifndef KNAPSACK_RESTRICTEDSOLUTION_H
#define KNAPSACK_RESTRICTEDSOLUTION_H


#include "Solution.h"

class RestrictedSolution : public Solution
{
public:
  
  RestrictedSolution(Problem& problem): Solution(problem){};
  
  void generateSolutin() override;
  
  const std::vector<float> &getMinimalFunctionValues() const;
  
private:
  std::vector<float> minimalFunctionValues_;

};


#endif //KNAPSACK_RESTRICTEDSOLUTION_H
