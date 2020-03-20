//
// Created by rick on 12.03.20.
//

#include <queue>
#include "revDP.h"

revDP::revDP(const problem& Problem, std::vector<float> baseValues):
  elements_(Problem.getElements()),
  capacity_(Problem.getCapacity()),
  functionSubset_(Problem.getRestrictedFunctions()),
  numberOfFunctions_(Problem.getRestrictedFunctions().size())
  {
    baseValues.push_back(0);
    
    pruningValues_.resize(elements_.size());//todo: grenzen beachten
    
    pruningValues_[0]->emplace_back(baseValues);
  }

void revDP::run()
{
  int counter = 0;
  //! adds element to all allowed
  for (auto element = elements_.end(); element != elements_.begin(); --element)
  {
    pruningValues_[counter + 1]->resize(pruningValues_[counter]->size() * 2);
  
    for (auto oldSol: *pruningValues_[counter])
    {
      pruningValues_[counter + 1]->emplace_back(oldSol);
    }
    
    for (auto oldSol: *pruningValues_[counter])
    {
      if (oldSol.at(0) - element->at(0) >= 0)
      {
        std::vector<float> newSolution(numberOfFunctions_ + 2, 0);
    
        newSolution[0] = oldSol.at(0) - element->at(0);
    
        newSolution.back() = 0;
    
        for (int i = 1; i <= numberOfFunctions_; ++i)
        {
          newSolution[i] =
              oldSol.at(i) - element->at(functionSubset_[i]) < 0 ? 0 : oldSol.at(i) - element->at(functionSubset_[i]);
        }
        
        bool hasDominated = false;
        
        bool wasDominated = false;
        
        for (auto sol = pruningValues_[counter + 1]->begin(); sol != pruningValues_[counter + 1]->end(); ++sol)
        {
          switch(compareInterval(*sol, newSolution))
          {
            case interval::disjunct:
            {
              continue;
            }
            
            case interval::left:
            {
              if(dominates(*sol, newSolution))
              {
                if(newSolution.back() <= sol->back())
                {
                  newSolution.back() = sol->back() + 1;
                }
              }
              if(dominates(newSolution, *sol))
              {
                sol->front() = -1;
                sol->back() = -1;
                hasDominated = true;
              }
              break;
            }
            
            case interval::right:
            {
              if(!hasDominated and dominates(*sol, newSolution))
              {
                wasDominated = true;
              }
              if(dominates(newSolution, *sol))
              {
                sol->back() = newSolution[0] + 1;
                
                hasDominated = true;
              }
              break;
            }
              
            case interval::equal:
            {
              if(dominates(*sol, newSolution))
              {
                wasDominated = true;
              }
              if(dominates(newSolution, *sol))
              {
                sol->front() = -1;
                sol->back() = -1;
              }
              break;
            }
          }
          
          if(wasDominated)
          {
            break;
          }
        }
        if(!wasDominated)
        {
          pruningValues_[counter + 1]->emplace_back(newSolution);
        }
      }
    }
    
    std::vector<std::vector<float>> nonDominatedSol(pruningValues_[counter + 1]->size(), std::vector<float>(numberOfFunctions_ + 2));
    
    for (auto sol : *pruningValues_[counter + 1])
    {
      if(sol[0] != -1)
      {
        nonDominatedSol.emplace_back(sol);
      }
    }
  
    pruningValues_[counter + 1] = &nonDominatedSol;
    
    ++counter;
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


interval revDP::compareInterval(std::vector<float> &oldSol, std::vector<float> &newSol)
{
  if(oldSol[0] < newSol.back() or newSol[0] < oldSol.back())
  {
    return interval::disjunct;
  }
  
  if (oldSol[0] == newSol[0])
  {
    return interval::equal;
  }
  
  if(oldSol[0] >= newSol.back())
  {
    return interval::left;
  }
  
  if(oldSol.back() <= newSol[0])
  {
    return interval::right;
  }
  
}