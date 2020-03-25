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
    baseValues.emplace(baseValues.begin(), capacity_);
  
    baseValues.push_back(0);
    
    pruningValues_.resize(elements_.size());//todo: grenzen beachten
    
    pruningValues_[0] = new std::vector<std::vector<float>>;
  
    pruningValues_[0]->emplace_back(baseValues);
  }

std::vector<std::vector<std::vector<float>> *> revDP::run()
{
  int counter = 0;
  //! adds element to all allowed
  for (auto element = elements_.rbegin(); element != elements_.rend(); ++element)
  {
    pruningValues_[counter + 1] = new std::vector<std::vector<float>>;
  
    pruningValues_[counter + 1]->reserve(pruningValues_[counter]->size() * 2);
  
    for (auto oldSol: *pruningValues_[counter])
    {
      pruningValues_[counter + 1]->emplace_back(oldSol);
    }
  
    int numberOfDominatedSolutions = 0;
    
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
              oldSol.at(i) - element->at(functionSubset_[i - 1]) < 0 ? 0 : oldSol.at(i) - element->at(functionSubset_[i - 1]);
        }
        
        bool hasDominated = false;
        
        bool wasDominated = false;
        
        if(newSolution[2] == 0 and newSolution[1] == 0)
        {
          int i = newSolution[2];
        }
        
        for (auto sol = pruningValues_[counter + 1]->begin(); sol != pruningValues_[counter + 1]->end(); ++sol)
        {
          if(sol->back() == -1)
          {
            continue;
          }
          
          if(same(*sol, newSolution))
          {
            sol->back() = std::min(sol->back(), newSolution.back());
            sol->front() = std::max(sol->front(), newSolution.front());
            wasDominated = true;
            continue;
          }
          
          switch(compareInterval(*sol, newSolution))
          {
            case interval::disjunct:
            {
              continue;
            }
            
            case interval::capacityLower:
            {
              if(dominates(*sol, newSolution))
              {
                if(newSolution.back() <= sol->front())
                {
                  newSolution.back() = sol->front() + 1;
                }
              }
              if(dominates(newSolution, *sol))
              {
                sol->front() = -1;
                sol->back() = -1;
                ++numberOfDominatedSolutions;
                hasDominated = true;
              }
              break;
            }
            
            case interval::capacityHigher:
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
              
            case interval::capacityEqual:
            {
              if(dominates(*sol, newSolution))
              {
                wasDominated = true;
              }
              if(dominates(newSolution, *sol))
              {
                sol->front() = -1;
                sol->back() = -1;
                ++numberOfDominatedSolutions;
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
  
    auto nonDominatedSol = new std::vector<std::vector<float>> (pruningValues_[counter + 1]->size() - numberOfDominatedSolutions, std::vector<float>(numberOfFunctions_ + 2));

    int pos = 0;
    
    for (auto & i : *pruningValues_[counter + 1])
    {
      if(i[0] != -1)
      {
        (*nonDominatedSol)[pos] = i;
        ++pos;
      }
    }
    auto test = pruningValues_[counter + 1];
    
    test->clear();
  
    pruningValues_[counter + 1] = nonDominatedSol;
    
    delete test;
    
    ++counter;
  }
  return pruningValues_;
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
    return interval::capacityEqual;
  }
  
  if (oldSol[0] > newSol[0])
  {
    if (oldSol.back() <= newSol[0])
    {
      return interval::capacityHigher;
    }
  }
  
  if (oldSol[0] >= newSol.back())
  {
    return interval::capacityLower;
  }
  
}

bool revDP::same(std::vector<float> &sol1, std::vector<float> &sol2)
{
  bool test = true;
  
  for (auto i: functionSubset_)
  {
    if(sol1[i] != sol2[i])
    {
      test = false;
      break;
    }
  }
  return test;
}

//todo: abbruch bei alles 0 und überlappen bei mehrfachen ausführen und vector