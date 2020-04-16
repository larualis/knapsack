//
// Created by rick on 11.03.20.
//

#ifndef KNAPSACK_DP_H
#define KNAPSACK_DP_H


#include <vector>
#include <list>
#include "Problem.h"
#include "revDP.h"
#include <memory>

class DP
{
private:
  const Problem& problem_;
  
  ElementManager elementManager_;
  
  float capacity_;
 
  int numberOfFunctions_;
  
  std::vector<int> functionsToCompare_;
  
  std::vector<int> functionsRestricted_;
  
  std::vector<std::vector<float>> solutions_;
  
  std::vector<int> validRounds_;
  
  std::list<std::vector<float>> finalSol;
public:
  const std::list<std::vector<float>> &getFinalSol() const;

private:
  
  std::vector<std::shared_ptr<std::vector<PruningSolution>>> pruningValues_;
  
public:
  DP(const Problem& problem, std::vector<int> functionsToCompare);
  
  DP(const Problem &problem, std::vector<std::shared_ptr<std::vector<PruningSolution>>> pruningValues);
  
  void run();
  
  const std::vector<std::vector<float>> &getSolutions() const;

private:
  
  bool dominates(std::vector<float>& sol1, std::vector<float>& sol2, bool lastElement);
  
  bool isValidAccordingToPruning(std::vector<float> &sol, int counter, int starValue, int &validForRounds);
  
  bool dlex(std::vector<float> &sol1, std::vector<float> &sol2);
  
  bool lex(std::vector<float> &sol1, std::vector<float> &sol2);
  
  void maintainNonDominated(std::vector<float>& newSolution, int validRound, std::list<std::vector<float>>& compareSol, int counter, int startValue);
  
  void keepNonDominated(std::vector<float> &newSolution, std::list<std::vector<float>> &compareSol);
  
  std::vector<float> upperBound(std::vector<float> &sol);
};


#endif //KNAPSACK_DP_H
