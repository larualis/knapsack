//
// Created by rick on 12.03.20.
//

#include <queue>
#include <iostream>
#include "revDP.h"

revDP::revDP(const Problem& problem, std::vector<float> baseValues):
  elements_(problem.getElements()),
  capacity_(problem.getCapacity()),
  functionSubset_(problem.getRestrictedFunctions()),
  numberOfFunctions_(problem.getRestrictedFunctions().size())
  {
    baseValues.emplace(baseValues.begin(), capacity_);
    
    pruningValues_.reserve(elements_.size());
  
    std::shared_ptr<std::vector<PruningSolution>> shr = std::make_shared<std::vector<PruningSolution>>();
    
    pruningValues_.emplace_back(shr);
  
    pruningValues_[0]->emplace_back(PruningSolution(baseValues));
  }

std::vector<std::shared_ptr<std::vector<PruningSolution>>> revDP::run()
{
  int numberOfCurrentElement = 0;
  
  float weightOfRemainingElements = 0;
  
  for (const auto &element : elements_)
  {
    weightOfRemainingElements += element.at(0);
  }
  
  for (auto element = elements_.rbegin(); element != elements_.rend(); ++element)
  {
    //init
    ++numberOfCurrentElement;
    
    std::cout<<numberOfCurrentElement<<"\n";
  
    int numberOfPreviousSolutions = pruningValues_[numberOfCurrentElement - 1]->size();
  
    std::shared_ptr<std::vector<PruningSolution>> shr = std::make_shared<std::vector<PruningSolution>>();
  
    pruningValues_.emplace_back(shr);
    
    pruningValues_[numberOfCurrentElement]->reserve(numberOfPreviousSolutions * 2);
  
    std::shared_ptr<std::vector<PruningSolution>> currentSolution = pruningValues_[numberOfCurrentElement];
  
    std::shared_ptr<std::vector<PruningSolution>> previousSolution = pruningValues_[numberOfCurrentElement - 1];
  
    int posNewSolutions = 0; // i
    
    int posOldSolutions = 0; // j
  
    std::list<PruningSolution*> compareSol;
  
    std::list<PruningSolution> equalWeightStack;
    //init end
    
    weightOfRemainingElements -= element->at(0);
  
    while (posNewSolutions < numberOfPreviousSolutions and (*previousSolution)[posNewSolutions].weight_ - element->at(0) >= 0)
    {
      std::vector<float> newSolutionVec(numberOfFunctions_ + 1, 0);
  
      newSolutionVec[0] = (*previousSolution)[posNewSolutions].weight_ - element->at(0); //todo: doppelte berechnung
  
      for (int i = 1; i <= numberOfFunctions_; ++i)
      {
        newSolutionVec[i] =
            (*previousSolution)[posNewSolutions].solutionValues_.at(i) - element->at(functionSubset_[i - 1]) < 0 ? 0 : (*previousSolution)[posNewSolutions].solutionValues_.at(i) - element->at(functionSubset_[i - 1]);
      }
      
      PruningSolution newSolution(newSolutionVec);
    
      while (posOldSolutions < numberOfPreviousSolutions and (*previousSolution)[posOldSolutions].weight_ >= newSolution.weight_)
      {
        maintainNonDominated((*previousSolution)[posOldSolutions], compareSol, equalWeightStack, currentSolution);
        ++posOldSolutions;
      }

      maintainNonDominated(newSolution, compareSol, equalWeightStack, currentSolution);
    
      ++posNewSolutions;
    }
  
    while(posOldSolutions < numberOfPreviousSolutions)
    {
      maintainNonDominated((*previousSolution)[posOldSolutions], compareSol, equalWeightStack, currentSolution);
    
      ++posOldSolutions;
    }
    
    if (!equalWeightStack.empty())
    {
      float oldWeight = equalWeightStack.front().weight_;
      
      for (auto &sol: compareSol)
      {
        if(sol->weight_ == oldWeight)
        {
          currentSolution->push_back(*sol);
        }
      }
    }
    
    if(weightOfRemainingElements == 0)
    {
      auto lastEle = currentSolution->back();
      continue;
    }
    pruningValues_[numberOfCurrentElement]->shrink_to_fit();
  }
  return pruningValues_;
}

void revDP::maintainNonDominated(PruningSolution &newSolution, std::list<PruningSolution*> &compareSol, std::list<PruningSolution> &equalWeightStack, std::shared_ptr<std::vector<PruningSolution>> currentSolution)
{
  bool newSolutionIsGood = false;
  
  if (compareSol.empty())
  {
    equalWeightStack.push_back(newSolution);
  
    compareSol.push_back(&equalWeightStack.back());
    
    return;
  }
  
  if(!equalWeightStack.empty())
  {
    float oldWeight = equalWeightStack.front().weight_;
  
    if(newSolution.weight_ != oldWeight)
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
    if(!newSolutionIsGood and !dlex((**sol).solutionValues_, newSolution.solutionValues_))
    {
      newSolutionIsGood = true;
      
      newSolution.relevantRounds_ += 1;
  
      equalWeightStack.push_back(newSolution);
      
      sol = compareSol.insert(sol, &equalWeightStack.back());
      
      ++sol;
    }
    
    if(!newSolutionIsGood and dominates((**sol).solutionValues_, newSolution.solutionValues_))
    {
      return;
    }
    
    if(newSolutionIsGood)
    {
      if(dominates(newSolution.solutionValues_, (**sol).solutionValues_))
      {
        (**sol).lowCapacity_ = newSolution.weight_ + 1;
        
        sol = compareSol.erase(sol);
        --sol;
      }
    }
  }
  
  if(!newSolutionIsGood)
  {
    equalWeightStack.push_back(newSolution);
    
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