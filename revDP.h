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
  std::vector<float> solutionValues_;
  
  float weight_;
  
  float lowCapacity_;
  
  int relevantRounds_;
  
  explicit PruningSolution(std::vector<float> solution)
  {
    solutionValues_ = std::move(solution);
  
    weight_ = solutionValues_.front();
    
    lowCapacity_ = 0;
  
    relevantRounds_ = -1;
  }
};

class revDP
{

private:
  const std::vector<std::vector<float>>& elements_;
  
  float capacity_;
  
  int numberOfFunctions_;
  
  std::vector<int> functionSubset_;
  
  std::vector<std::shared_ptr<std::vector<PruningSolution>>> pruningValues_;
public:
  
  revDP(const Problem& problem, std::vector<float> baseValues);
  
  std::vector<std::shared_ptr<std::vector<PruningSolution>>> run();
private:
  
  bool dominates(std::vector<float>& sol1, std::vector<float>& sol2);
  
  bool dlex(std::vector<float> &sol1, std::vector<float> &sol2);
  
  void maintainNonDominated(PruningSolution &newSolution, std::list<PruningSolution *> &compareSol, std::list<PruningSolution> &equalWeightStack,
                            std::shared_ptr<std::vector<PruningSolution>> currentSolution);
};


#endif //KNAPSACK_REVDP_H
