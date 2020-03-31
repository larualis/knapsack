//
// Created by rick on 12.03.20.
//

#include <queue>
#include <iostream>
#include "revDP.h"

revDP::revDP(const problem& Problem, std::vector<float> baseValues):
  elements_(Problem.getElements()),
  capacity_(Problem.getCapacity()),
  functionSubset_(Problem.getRestrictedFunctions()),
  numberOfFunctions_(Problem.getRestrictedFunctions().size())
  {
    baseValues.emplace(baseValues.begin(), capacity_);
  
    baseValues.push_back(0);
    
    pruningValues_.resize(elements_.size());//todo: grenzen beachten
    
    pruningValues_[0] = new std::vector<std::vector<float>>;
  
    pruningValues_[0]->emplace_back(baseValues);
  }

std::vector<std::vector<std::vector<float>> *> revDP::run()
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
  
    pruningValues_[numberOfCurrentElement] = new std::vector<std::vector<float>>;
  
    pruningValues_[numberOfCurrentElement]->reserve(numberOfPreviousSolutions * 2);
    
    std::vector<std::vector<float>>* currentSolution = pruningValues_[numberOfCurrentElement];
  
    std::vector<std::vector<float>>* previousSolution = pruningValues_[numberOfCurrentElement - 1];
  
    int posNewSolutions = 0; // i
    
    int posOldSolutions = 0; // j
  
    std::list<std::vector<float>*> compareSol;
    //init end
    
    weightOfRemainingElements -= element->at(0);
  
    while (posNewSolutions < numberOfPreviousSolutions and (*previousSolution)[posNewSolutions][0] - element->at(0) >= 0)
    {
      std::vector<float> newSolution(numberOfFunctions_ + 2, 0);
  
      newSolution[0] = (*previousSolution)[posNewSolutions][0] - element->at(0); //todo: doppelte berechnung
  
      for (int i = 1; i <= numberOfFunctions_; ++i)
      {
        newSolution[i] =
            (*previousSolution)[posNewSolutions].at(i) - element->at(functionSubset_[i - 1]) < 0 ? 0 : (*previousSolution)[posNewSolutions].at(i) - element->at(functionSubset_[i - 1]);
      }
    
      while (posOldSolutions < numberOfPreviousSolutions and (*previousSolution)[posOldSolutions].front() >= newSolution.front())
      {
        maintainNonDominated((*previousSolution)[posOldSolutions], compareSol, currentSolution);
        ++posOldSolutions;
      }

      maintainNonDominated(newSolution, compareSol, currentSolution);
    
      ++posNewSolutions;
    }
  
    while(posOldSolutions < numberOfPreviousSolutions)
    {
      maintainNonDominated((*previousSolution)[posOldSolutions], compareSol, currentSolution);
    
      ++posOldSolutions;
    }
    if(weightOfRemainingElements == 0)
    {
      auto lastEle = currentSolution->back();
      continue;
    }
  }
  return pruningValues_;
}

void revDP::maintainNonDominated(std::vector<float> &newSolution, std::list<std::vector<float>*> &compareSol, std::vector<std::vector<float>>* currentSolution)
{
  bool newSolutionIsGood = false;
  
  bool dominated = false;
  
  if (compareSol.empty())
  {
    currentSolution->push_back(newSolution);
  
    compareSol.push_back(&currentSolution->back());
    
    return;
  }
  
  for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
  {
    if(!newSolutionIsGood and !dlex(**sol, newSolution))
    {
      newSolutionIsGood = true;
  
      currentSolution->push_back(newSolution);
      
      sol = compareSol.insert(sol, &currentSolution->back());
      
      ++sol;
    }
    
    if(!newSolutionIsGood and dominates(**sol, newSolution))
    {
      dominated = true;
      break;
    }
    
    if(newSolutionIsGood)
    {
      if(dominates(newSolution, **sol))
      {
        (**sol).back() = newSolution[0] + 1;
        
        sol = compareSol.erase(sol);
        --sol;
      }
    }
  }
  
  if(!dominated and !newSolutionIsGood)
  {
    currentSolution->push_back(newSolution);
    
    compareSol.push_back(&currentSolution->back());
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