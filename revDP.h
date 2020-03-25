//
// Created by rick on 12.03.20.
//

#ifndef KNAPSACK_REVDP_H
#define KNAPSACK_REVDP_H

#import <vector>
#import <list>
#include "problem.h"

enum class interval : int
{
  disjunct,
  capacityLower,
  capacityHigher,
  capacityEqual
};

class revDP
{

private:
  const std::vector<std::vector<float>>& elements_;
  
  float capacity_;
  
  int numberOfFunctions_;
  
  std::vector<int> functionSubset_;
  
  std::vector<std::vector<std::vector<float> >* > pruningValues_;
public:
  
  revDP(const problem& Problem, std::vector<float> baseValues);
  
  std::vector<std::vector<std::vector<float>> *> run();
private:
  
  bool dominates(std::vector<float>& sol1, std::vector<float>& sol2);
  
  interval compareInterval(std::vector<float> &sol1, std::vector<float> &sol2);
  
  bool same(std::vector<float> &sol1, std::vector<float> &sol2);
};


#endif //KNAPSACK_REVDP_H
