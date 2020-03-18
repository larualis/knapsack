//
// Created by rick on 12.03.20.
//

#include "revDP.h"

revDP::revDP(const problem& Problem, std::vector<float> baseValues):
  elements_(Problem.getElements()),
  capacity_(Problem.getCapacity()),
  numberOfFunctions_(Problem.getRestrictedFunctions().size())
  {
    for (auto x: Problem.getRestrictedFunctions())
    {
      functionSubset_.push_back(x.first);
    }
  
    baseValues.push_back(0);
    
    rawPruningValues_.push_back(baseValues);
    
    pruningValues_.resize(elements_.size());//todo: grenzen beachten
    
    pruningValues_[0].push_back( &rawPruningValues_.front());
  }

void revDP::run()
{
  int counter = 0;
  //! adds element to all allowed
  for (auto element = elements_.end(); element != elements_.begin(); --element)
  {
    pruningValues_[counter + 1] = pruningValues_[counter];
    
    for (auto oldSol: pruningValues_[counter])
    {
      if (oldSol->at(0) - element->at(0) >= 0)
      {
        std::vector<float> newSolution(numberOfFunctions_ + 2, 0);
        
        newSolution[0] = oldSol->at(0) - element->at(0);
        
        newSolution[newSolution.size() - 1] = 0;
  
        for(int i = 1; i <= numberOfFunctions_ ; ++i )
        {
          newSolution[i] = oldSol->at(i) - element->at(functionSubset_[i]);
        }
        
        bool hasDominated = false;
        
        bool wasDominated = false;
        
        for (auto sol = pruningValues_[counter + 1].begin(); sol != pruningValues_[counter + 1].end(); ++sol)
        {
          if((**sol).back() > newSolution[0])
          {
            rawPruningValues_.emplace_back(newSolution);
            
            pruningValues_[counter + 1].insert(sol, &rawPruningValues_.back());
            break;
          }
          
          switch(compareInterval(**sol, newSolution))
          {
            case interval::disjunct:
            {
              continue;
            }
            
            case interval::left:
            {
              if(dominates(**sol, newSolution))
              {
                if(newSolution.back() <= (**sol).back())
                {
                  newSolution.back() = (**sol).back() + 1;
                }
              }
              if(dominates(newSolution, **sol))
              {
                pruningValues_[counter + 1].erase(sol);
                hasDominated = true;
              }
              break;
            }
            
            case interval::right:
            {
              if(!hasDominated and dominates(**sol, newSolution))
              {
                wasDominated = true;
              }
              if(dominates(newSolution, **sol))
              {
                rawPruningValues_.push_back(**sol);
                
                rawPruningValues_.back().back() = newSolution[0] + 1;
                
                pruningValues_[counter + 1].insert(sol, &rawPruningValues_.back());
  
                hasDominated = true;
              }
              break;
            }
              
            case interval::equal:
            {
              if(dominates(**sol, newSolution))
              {
                wasDominated = true;
              }
              if(dominates(newSolution, **sol))
              {
                pruningValues_[counter + 1].erase(sol);
              }
              break;
            }
          }
          
          if(wasDominated)
          {
            break;
          }
        }
      }
    }
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
  if(oldSol[0] < newSol.back())
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