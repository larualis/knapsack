//
// Created by rick on 01.06.20.
//

#ifndef KNAPSACK_APPROXPRUNING_H
#define KNAPSACK_APPROXPRUNING_H


#include "Pruning.h"
#include <cmath>

class ApproxPruning : public Pruning
{
public:
  ApproxPruning(int numberOfElements, int numberOfFunctions, int frequency, double error);
  
  ApproxPruning(int numberOfElements, std::vector<int> functionsWhereErrorNotIsUsed,  int numberOfFunctions, int frequency, double error);
  
  bool shouldSolutionBeRemoved(static_vector& solution);
  
  void updateListOfBestSolutions(static_vector& solution);
  
  void setupForNextElement(std::list<static_vector>* currentBestSolutions, int numberOfCurrentElement);
  
  void errorWasNotUsed();
  
private:
  
  std::list<static_vector>* currentBestSolutions_;
  
  int numberOfElements_;
  
  double error_ = 0.1;
  
  int frequency_;
  
  double sumOfErrorsNotUsed_;
  
  int numberOfRoundsWhereErrorWillBeUsed_;
  
  double errorThisRound_;
  
  double errorCalculatedForDomination_;
  
  std::vector<int> functionsWhereErrorIsUsed_;
  
  std::vector<int> functionsWhereErrorIsNotUsed_;
  
  bool dominatesWithError(static_vector &sol1, static_vector &sol2);
};

inline bool ApproxPruning::dominatesWithError(static_vector &sol1, static_vector &sol2)
{
  for(auto i: functionsWhereErrorIsUsed_)
  {
    int valForSol1i = sol1[i] != 0 ? (int)(log2(sol1[i]) / errorCalculatedForDomination_) + 1 : 0;
    
    int valForSol12 = sol2[i] != 0 ? (int)(log2(sol2[i]) / errorCalculatedForDomination_) + 1 : 0;
    
    if( valForSol1i < valForSol12)
    {
      return false;
    }
  }
  
  for(auto i : functionsWhereErrorIsNotUsed_)
  {
    if(sol1[i] < sol2[i])
    {
      return false;
    }
  }
  
  return true;
}



#endif //KNAPSACK_APPROXPRUNING_H
