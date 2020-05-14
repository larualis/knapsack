//
// Created by rick on 12.03.20.
//

#ifndef KNAPSACK_REVDP_H
#define KNAPSACK_REVDP_H

#include <vector>
#include <list>
#include "Problem.h"
#include <memory>

struct PruningSolution
{
  static_vector solutionValues_;
  
  int weight_;
  
  int lowCapacity_;
  //! number of rounds to come, where this is optimal , 0 means last round
  int relevantRounds_;
  
  explicit PruningSolution(static_vector solution):
  solutionValues_(solution.begin() + 1,solution.end())
  {
    weight_ = solution.front();
    
    lowCapacity_ = 0;
  
    relevantRounds_ = -1;
  }
  
  explicit PruningSolution(int numberOfFunctions):
  solutionValues_(numberOfFunctions, 0)
  {
    weight_ = 0;
    
    lowCapacity_ = 0;
    
    relevantRounds_ = -1;
  }
};

class revDP
{

private:
  Problem& problem_;
  
  ElementManager& elementManager_;
  
  int capacity_;
  
  int numberOfFunctions_;
  
  std::vector<int> functionsRestricted_;
  
  std::vector<std::vector<PruningSolution>> pruningValues_;
public:
  std::vector<std::vector<PruningSolution>> &getPruningValues();

public:
  
  revDP(Problem& problem, std::vector<int> baseValues);
  
  void run();
  
private:
  
  bool dominates(PruningSolution& sol1, PruningSolution& sol2);
  
  bool dlex(PruningSolution& sol1, PruningSolution& sol2);
  
  void maintainNonDominated(PruningSolution &solution, std::list<PruningSolution *> &compareSol, std::list<PruningSolution> &equalWeightStack,
                            std::vector<PruningSolution>* currentSolution);
};


#endif //KNAPSACK_REVDP_H
