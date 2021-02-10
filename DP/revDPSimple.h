//
// Created by rick on 14.09.20.
//

#ifndef KNAPSACK_REVDPSIMPLE_H
#define KNAPSACK_REVDPSIMPLE_H

#include <vector>
#include <list>
#include "../Problem.h"
#include "../pruning_rules/ParetoPruning.h"
#include <memory>
#include "../revDP.h"

class revDPSimple
{

private:
  Problem& problem_;
  
  ElementManager& elementManager_;
  
  ParetoPruning paretoPruner_;
  
  int capacity_;
  
  int numberOfFunctions_;
  
  std::vector<int> functionsRestricted_;
  
  std::vector<int> minimalValues_;
  
  std::vector<std::vector<PruningSolution>> pruningValues_;
public:

  std::vector<std::vector<PruningSolution>> &getPruningValues();
  
  revDPSimple(Problem& problem, std::vector<int> baseValues);
  
  void run();

private:
  
  bool lex(PruningSolution& sol1, PruningSolution& sol2);
  
  bool dlex(PruningSolution& sol1, PruningSolution& sol2);
  
  void maintainNonDominated(PruningSolution &solution, std::vector<PruningSolution>* solutionsThisRound_);
};

#endif //KNAPSACK_REVDPSIMPLE_H
