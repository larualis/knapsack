//
// Created by rick on 11.03.20.
//

#ifndef KNAPSACK_DP_H
#define KNAPSACK_DP_H


#include <vector>
#include <list>
#include "problem.h"

class DP
{
private:
  const std::vector<std::vector<float>>& elements_;
 
  float capacity_;
 
  int numberOfFunctions_;
  
  std::vector<int> functionsToCompare_;
  
  std::vector<int> functionsRestricted_;
  
  std::vector<std::vector<float>> solutions_;
  
  std::list<std::vector<float>> finalSol;
public:
  const std::list<std::vector<float>> &getFinalSol() const;

private:
  
  std::vector<std::vector<std::vector<float>>*> pruningValues_;
  
public:
  DP(const problem& Problem, std::vector<int> functionsToCompare);
  
  DP(const problem &Problem, std::vector<std::vector<std::vector<float>> *> pruningValues);
  
  void run();
  
  const std::vector<std::vector<float>> &getSolutions() const;

private:
  
  bool dominates(std::vector<float>& sol1, std::vector<float>& sol2, bool lastElement);
  
  bool isValidAccordingToPruning(std::vector<float> &sol, int counter);
  
  bool dlex(std::vector<float> &sol1, std::vector<float> &sol2);
  
  bool lex(std::vector<float> &sol1, std::vector<float> &sol2);
  
  void maintainNonDominated(std::vector<float>& newSolution, std::list<std::vector<float>>& compareSol, int counter);
};


#endif //KNAPSACK_DP_H
