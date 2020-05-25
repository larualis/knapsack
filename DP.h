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
  
  int capacity_;
 
  int numberOfFunctions_;
  
  int numberOfCurrentElement_;
  
  std::vector<int> functionsToCompare_;
  
  std::vector<int> functionsRestricted_;
  
  std::vector<static_vector> solutions_;
  
  std::vector<int> validRounds_;
  
  float error_;

private:
  
  std::vector<std::vector<PruningSolution>> pruningValues_;
  
public:
  DP(Problem& problem, std::vector<int> functionsToCompare, float error=0);
  
  DP(Problem &problem, std::vector<std::vector<PruningSolution>>& pruningValues, float error=0);
  
  void run();
  
  const std::vector<static_vector>& getSolutions() const;

private:
  
  bool dominates(static_vector& sol1, static_vector& sol2, bool lastElement);
  
  bool isValidAccordingToPruning(static_vector& sol, int counter, int& starValue, int &validForRounds);
  
  bool dlex(static_vector& sol1, static_vector& sol2);
  
  bool lex(static_vector& sol1, static_vector& sol2);
  
  void maintainNonDominated(static_vector& solution, int validRound, std::list<static_vector>& currentBestSolutions, int counter, int& startValue);
  
  void keepNonDominated(static_vector& newSolution, std::list<static_vector>& currentBestSolutions);
  
  int removeWithBetterExtension(std::list<static_vector>& currentBestSolutions);
  
  static_vector upperBound(static_vector &sol);
};


#endif //KNAPSACK_DP_H
