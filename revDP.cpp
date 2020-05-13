//
// Created by rick on 12.03.20.
//

#include <queue>
#include <iostream>
#include "revDP.h"

revDP::revDP(Problem& problem, std::vector<float> baseValues):
  problem_(problem),
  elementManager_(problem.getEleManager()),
  capacity_(problem.getCapacity()),
  functionSubset_(problem.getRestrictedFunctions()),
  numberOfFunctions_(problem.getRestrictedFunctions().size())
  {
    baseValues.emplace(baseValues.begin(), capacity_);
    
    pruningValues_.reserve(elementManager_.getNumberOfElements() + 1);
  
    std::shared_ptr<std::vector<PruningSolution>> shr = std::make_shared<std::vector<PruningSolution>>();
    
    pruningValues_.emplace_back(shr);
  
    pruningValues_[0]->emplace_back(PruningSolution(baseValues));
  }

std::vector<std::shared_ptr<std::vector<PruningSolution>>> revDP::run()
{
  int numberOfCurrentElement = 0;
  
  float weightOfRemainingElements = problem_.getSumOfWeights();
  
  for (auto element = elementManager_.getElements().rbegin(); element != elementManager_.getElements().rend(); ++element)
  {
    //init
    ++numberOfCurrentElement;
  
    std::cout<<numberOfCurrentElement<<"\r"<<std::flush;
  
    int numberOfPreviousSolutions = pruningValues_[numberOfCurrentElement - 1]->size();
  
    std::shared_ptr<std::vector<PruningSolution>> shr = std::make_shared<std::vector<PruningSolution>>();
  
    pruningValues_.emplace_back(shr);
    
    pruningValues_[numberOfCurrentElement]->reserve(numberOfPreviousSolutions * 2);
  
    std::shared_ptr<std::vector<PruningSolution>> solutionsThisRound = pruningValues_[numberOfCurrentElement];
  
    std::shared_ptr<std::vector<PruningSolution>> solutionsLastRound = pruningValues_[numberOfCurrentElement - 1];
  
    //! i in paper
    int idxNewSolutions = 0;
  
    //! j in paper
    int idxOldSolutions = 0;
  
    std::list<PruningSolution*> compareSol;
  
    std::list<PruningSolution> equalWeightStack;
    //init end
    
    weightOfRemainingElements -= element->weight_;
  
    while (idxNewSolutions < numberOfPreviousSolutions and (*solutionsLastRound)[idxNewSolutions].weight_ - element->weight_ >= 0)
    {
      std::vector<float> newSolutionVec(numberOfFunctions_ + 1, 0);
  
      newSolutionVec[0] = (*solutionsLastRound)[idxNewSolutions].weight_ - element->weight_; //todo: doppelte berechnung
  
      for (int i = 1; i <= numberOfFunctions_; ++i)
      {
        newSolutionVec[i] =
            (*solutionsLastRound)[idxNewSolutions].solutionValues_.at(i) - element->values_.at(functionSubset_[i - 1] - 1) < 0 ? 0 : (*solutionsLastRound)[idxNewSolutions].solutionValues_.at(i) - element->values_.at(functionSubset_[i - 1] - 1);
      }
      
      PruningSolution newSolution(newSolutionVec);
    
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
      float oldWeight = equalWeightStack.front().weight_;
      
      for (auto &sol: compareSol)
      {
        if(sol->weight_ == oldWeight)
        {
          solutionsThisRound->push_back(*sol);
        }
      }
    }
    
    pruningValues_[numberOfCurrentElement]->shrink_to_fit();
  }
  return pruningValues_;
}

void revDP::maintainNonDominated(PruningSolution &solution, std::list<PruningSolution*> &compareSol, std::list<PruningSolution> &equalWeightStack, std::shared_ptr<std::vector<PruningSolution>> currentSolution)
{
  bool newSolutionIsGood = false;
  
  if (compareSol.empty())
  {
    equalWeightStack.push_back(solution);
  
    compareSol.push_back(&equalWeightStack.back());
    
    return;
  }
  
  if(!equalWeightStack.empty())
  {
    float oldWeight = equalWeightStack.front().weight_;
  
    if(solution.weight_ != oldWeight)
    {
      for (auto &sol: compareSol)
      {
        if(sol->weight_ == oldWeight)
        {
          currentSolution->push_back(*sol);//todo nur wenn vorderes interval nicht größer ist als capacity der verbleibenden elemente
        
          sol = &currentSolution->back();
        }
      }
    
      equalWeightStack.clear();
    }
  }
  
  for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
  {
    if(!newSolutionIsGood and !dlex((**sol).solutionValues_, solution.solutionValues_))
    {
      newSolutionIsGood = true;
  
      solution.relevantRounds_ += 1;
  
      equalWeightStack.push_back(solution);
      
      sol = compareSol.insert(sol, &equalWeightStack.back());
      
      ++sol;
    }
    
    if(!newSolutionIsGood and dominates((**sol).solutionValues_, solution.solutionValues_))
    {
      return;
    }
    
    if(newSolutionIsGood)
    {
      if(dominates(solution.solutionValues_, (**sol).solutionValues_))
      {
        (**sol).lowCapacity_ = solution.weight_ + 1;
        
        sol = compareSol.erase(sol);
        --sol;
      }
    }
  }
  
  if(!newSolutionIsGood)
  {
    equalWeightStack.push_back(solution);
    
    compareSol.push_back(&equalWeightStack.back());
  }
}

bool revDP::dominates(std::vector<float> &sol1, std::vector<float> &sol2)
{
  for (auto i: functionSubset_)
  {
    if(sol1[i] > sol2[i])
    {
      return false;
    }
  }
  return true;
}

bool revDP::dlex(std::vector<float> &sol1, std::vector<float> &sol2)
{
  for (int i = 1; i <= numberOfFunctions_; ++i)
  {
    if (sol1[i] != sol2[i])
    {
      return sol1[i] < sol2[i];
    }
  }
  return true;
}