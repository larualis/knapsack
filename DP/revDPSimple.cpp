//
// Created by rick on 14.09.20.
//

#include <iostream>
#include "revDPSimple.h"

revDPSimple::revDPSimple(Problem& problem, std::vector<int> baseValues):
    problem_(problem),
    elementManager_(problem.getEleManager()),
    paretoPruner_(problem.getNumberOfElements(), problem.getRestrictedFunctions().size()),
    capacity_(problem.getCapacity()),
    functionsRestricted_(problem.getRestrictedFunctions()),
    numberOfFunctions_(problem.getRestrictedFunctions().size()),
    minimalValues_(baseValues)
{
  pruningValues_.reserve(elementManager_.getNumberOfElements() + 1);
  
  pruningValues_.emplace_back(std::vector{PruningSolution(numberOfFunctions_)});
}

void revDPSimple::run()
{
  int numberOfCurrentElement = 0;
  
  int weightOfRemainingElements = problem_.getSumOfWeights();
  
  for (auto element = elementManager_.getElements().rbegin(); element != elementManager_.getElements().rend(); ++element)
  {
    //!init
    ++numberOfCurrentElement;
    
    weightOfRemainingElements -= element->weight_;
    
    std::cout<<numberOfCurrentElement<<"\r"<<std::flush;
    
    int numberOfPreviousSolutions = pruningValues_[numberOfCurrentElement - 1].size();
    
    pruningValues_.emplace_back(std::vector<PruningSolution>());
    
    pruningValues_[numberOfCurrentElement].reserve(numberOfPreviousSolutions * 2);
    
    std::vector<PruningSolution>* solutionsThisRound = &pruningValues_[numberOfCurrentElement];
    
    std::vector<PruningSolution>* solutionsLastRound = &pruningValues_[numberOfCurrentElement - 1];
  
    std::list<static_vector> currentBestSolutions;
  
    paretoPruner_.setupForNextElement(&currentBestSolutions);
    
    //! i in paper
    int idxNewSolution = 0;
    
    //! j in paper
    int idxOldSolution = 0;
    
    //!init end
    
    while (idxNewSolution < numberOfPreviousSolutions and (*solutionsLastRound)[idxNewSolution].weight_ <= capacity_)
    {
      PruningSolution newSolution(numberOfFunctions_);
  
      newSolution.weight_ = (*solutionsLastRound)[idxNewSolution].weight_ + element->weight_;
  
      for (int idx = 0; idx < numberOfFunctions_; ++idx)
      {
        newSolution.solutionValues_[idx] = (*solutionsLastRound)[idxNewSolution].solutionValues_[idx] + element->values_.at(functionsRestricted_[idx] - 1);
      }
      
      while (idxOldSolution < numberOfPreviousSolutions and lex((*solutionsLastRound)[idxOldSolution], newSolution))
      {
        maintainNonDominated((*solutionsLastRound)[idxOldSolution], solutionsThisRound);
        ++idxOldSolution;
      }
      
      maintainNonDominated(newSolution, solutionsThisRound);
      
      ++idxNewSolution;
    }
    
    while(idxOldSolution < numberOfPreviousSolutions)
    {
      maintainNonDominated((*solutionsLastRound)[idxOldSolution], solutionsThisRound);
      
      ++idxOldSolution;
    }
    //todo: man könnte hier noch die lösungen löschen, falls remainingcapacity < lowcapacity, da diese eh nie betrachtet werden
    solutionsThisRound->shrink_to_fit();
  }
  
  for(auto &x : pruningValues_)
  {
    for(auto &solution: x)
    {
      // negativität könnte bugs verursachen, notfalls 0 setzen bei minus
      for(int i = 0; i < numberOfFunctions_; ++i)
      {
        solution.solutionValues_[i] = minimalValues_[i] - solution.solutionValues_[i];
      }
      solution.weight_ = capacity_ - solution.weight_;
    }
  }
}

void revDPSimple::maintainNonDominated(PruningSolution &solution, std::vector<PruningSolution>* solutionsThisRound_)
{
  static_vector test({0});
  
  test.insert(test.end(), solution.solutionValues_.begin(), solution.solutionValues_.end());
  
  if(!paretoPruner_.shouldSolutionBeRemoved(test))
  {
    ++solution.relevantRounds_;
  
    (*solutionsThisRound_).push_back(solution);
    
    paretoPruner_.solutionWasAdded(test);
  }
}

bool revDPSimple::lex(PruningSolution& sol1, PruningSolution& sol2)
{
  if (sol1.weight_ < sol2.weight_)
  {
    return true;
  }
  
  return sol1.weight_ == sol2.weight_ and dlex(sol1, sol2);
}

bool revDPSimple::dlex(PruningSolution& sol1, PruningSolution& sol2)
{
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    if (sol1.solutionValues_[i] != sol2.solutionValues_[i])
    {
      return sol1.solutionValues_[i] > sol2.solutionValues_[i];
    }
  }
  return true;
}

std::vector<std::vector<PruningSolution>> &revDPSimple::getPruningValues()
{
  return pruningValues_;
}
