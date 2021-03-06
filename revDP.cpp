//
// Created by rick on 12.03.20.
//

#include <queue>
#include <iostream>
#include "revDP.h"

revDP::revDP(Problem& problem, std::vector<int> baseValues):
    problem_(problem),
    elementManager_(problem.getEleManager()),
    capacity_(problem.getCapacity()),
    functionsRestricted_(problem.getRestrictedFunctions()),
    numberOfFunctions_(problem.getRestrictedFunctions().size())
  {
    static_vector start(baseValues.begin(), baseValues.end());
    
    start.emplace(start.begin(), capacity_);
    
    pruningValues_.reserve(elementManager_.getNumberOfElements() + 1);
    
    pruningValues_.emplace_back(std::vector{PruningSolution(start)});
  }

void revDP::run()
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
  
    //! i in paper
    int idxNewSolutions = 0;
  
    //! j in paper
    int idxOldSolutions = 0;
  
    std::list<PruningSolution*> compareSol;
  
    std::list<PruningSolution> equalWeightStack;
    //!init end
    
    while (idxNewSolutions < numberOfPreviousSolutions and (*solutionsLastRound)[idxNewSolutions].weight_ - element->weight_ >= 0)
    {
      PruningSolution newSolution(numberOfFunctions_);
      
      newSolution.weight_ = (*solutionsLastRound)[idxNewSolutions].weight_ - element->weight_;
      
      for (int i = 0; i < numberOfFunctions_; ++i)
      {
        int val = (*solutionsLastRound)[idxNewSolutions].solutionValues_.at(i) - element->values_.at(functionsRestricted_[i] - 1);
        
        if(val > 0)
        {
          newSolution.solutionValues_[i] = val;
        }
      }
      
      while (idxOldSolutions < numberOfPreviousSolutions and (*solutionsLastRound)[idxOldSolutions].weight_ >= newSolution.weight_)
      {
        maintainNonDominated((*solutionsLastRound)[idxOldSolutions], compareSol, equalWeightStack, solutionsThisRound);
        ++idxOldSolutions;
      }

      maintainNonDominated(newSolution, compareSol, equalWeightStack, solutionsThisRound);
    
      ++idxNewSolutions;
    }
  
    while(idxOldSolutions < numberOfPreviousSolutions)
    {
      maintainNonDominated((*solutionsLastRound)[idxOldSolutions], compareSol, equalWeightStack, solutionsThisRound);
    
      ++idxOldSolutions;
    }
    
    if (!equalWeightStack.empty())
    {
      int oldWeight = equalWeightStack.front().weight_;
      
      for (auto &sol: compareSol)
      {
        if(sol->weight_ == oldWeight)
        {
          ++sol->relevantRounds_;
  
          solutionsThisRound->push_back(*sol);
        }
      }
    }
    //todo: man könnte hier noch die lösungen löschen, falls remainingcapacity < lowcapacity, da diese eh nie betrachtet werden
    solutionsThisRound->shrink_to_fit();
  }
}

void revDP::maintainNonDominated(PruningSolution &solution, std::list<PruningSolution*> &compareSol, std::list<PruningSolution> &equalWeightStack, std::vector<PruningSolution>* currentSolution)
{
  bool addSolution = true;
  
  bool solutionWasAddedToCompareSol = false;
  
  if(!equalWeightStack.empty())
  {
    int oldWeight = equalWeightStack.front().weight_;
  
    if(solution.weight_ != oldWeight)
    {
      for (auto &sol: compareSol)
      {
        if(sol->weight_ == oldWeight)
        {
          ++sol->relevantRounds_;
          
          currentSolution->push_back(*sol);
        
          sol = &currentSolution->back();
        }
      }
      equalWeightStack.clear();
    }
  }
  
  if(!compareSol.empty())
  {
    for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
    {
      if(!solutionWasAddedToCompareSol and !dlex((**sol), solution))
      {
        solutionWasAddedToCompareSol = true;
        
        equalWeightStack.push_back(solution);
  
        sol = compareSol.insert(sol, &equalWeightStack.back());
  
        addSolution = false;
        
        ++sol;
      }
      
      if(!solutionWasAddedToCompareSol and dominates((**sol), solution))
      {
        addSolution = false;
        
        break;
      }
      
      if(solutionWasAddedToCompareSol)
      {
        if(dominates(solution, (**sol)))
        {
          (**sol).lowCapacity_ = solution.weight_ + 1;
          
          sol = compareSol.erase(sol);
          --sol;
        }
      }
    }
  }
  
  if(addSolution)
  {
    equalWeightStack.push_back(solution);
    
    compareSol.push_back(&equalWeightStack.back());
  }
}

bool revDP::dominates(PruningSolution& sol1, PruningSolution& sol2)
{
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    if(sol1.solutionValues_[i] > sol2.solutionValues_[i])
    {
      return false;
    }
  }
  return true;
}

bool revDP::dlex(PruningSolution& sol1, PruningSolution& sol2)
{
  for (int i = 0; i < numberOfFunctions_; ++i)
  {
    if (sol1.solutionValues_[i] != sol2.solutionValues_[i])
    {
      return sol1.solutionValues_[i] < sol2.solutionValues_[i];
    }
  }
  return true;
}

std::vector<std::vector<PruningSolution>> &revDP::getPruningValues()
{
  return pruningValues_;
}
