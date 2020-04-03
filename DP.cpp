//
// Created by rick on 11.03.20.
//

#include <iostream>
#include "DP.h"
#include "problem.h"
#include <list>

DP::DP(const problem &Problem, std::vector<std::vector<PruningSolution>*> pruningValues):
    elements_(Problem.getElements()),
    capacity_(Problem.getCapacity()),
    numberOfFunctions_(Problem.getNumberOfFunctions()),
    functionsRestricted_(Problem.getRestrictedFunctions()),
    pruningValues_(pruningValues)
{
  solutions_.emplace_back(std::vector<float> (numberOfFunctions_ + 1, 0));
  functionsToCompare_.reserve(numberOfFunctions_);
  for(int i = 1; i <= numberOfFunctions_; ++i)
  {
    functionsToCompare_.push_back(i);
  }
}

DP::DP(const problem &Problem, std::vector<int> functionsToCompare):
    elements_(Problem.getElements()),
    capacity_(Problem.getCapacity()),
    numberOfFunctions_(functionsToCompare.size()),
    functionsToCompare_(functionsToCompare)
{
  solutions_.emplace_back(std::vector<float> (numberOfFunctions_ + 1, 0));
}

void DP::run()
{
  int counter = 0;
  
  float weightOfRemainingElements = 0;
  
  for (const auto &element : elements_)
  {
    weightOfRemainingElements += element.at(0);
  }
  
  for (const auto & element : elements_)
  {
    counter++;
    
    std::cout<<counter<<"\n";
    
    int i = 0;
    
    int j = 0;
    
    int startValue = 0;
  
    std::vector<std::vector<float>> previousSolutions = std::move(solutions_);
    
    std::list<std::vector<float>> compareSol;
    
    while (j < previousSolutions.size() and previousSolutions[j][0] + weightOfRemainingElements <= capacity_)
    {
      ++j;
    }
    
    while (i < previousSolutions.size() and previousSolutions[i][0] + element.at(0) <= capacity_)
    {
      std::vector<float> newSolution(numberOfFunctions_ + 1, 0);
      
      newSolution[0] = previousSolutions[i][0] + element.at(0);
      
      for (int idx = 1; idx <= numberOfFunctions_; ++idx)
      {
        newSolution[idx] = previousSolutions[i][idx] + element.at(functionsToCompare_[idx - 1]);
      }
      
      while (j < previousSolutions.size() and lex(previousSolutions[j], newSolution))
      {
        maintainNonDominated(previousSolutions[j], compareSol, counter, startValue);
        
        ++j;
      }

      maintainNonDominated(newSolution, compareSol, counter, startValue);
      
      ++i;
    }
    
    while(j < previousSolutions.size())
    {
      maintainNonDominated(previousSolutions[j], compareSol, counter, startValue);
  
      ++j;
    }
  
    weightOfRemainingElements -= element.at(0);
    
    if(weightOfRemainingElements == 0)
    {
      finalSol = std::move(compareSol);
    }
  }
}


void DP::maintainNonDominated(std::vector<float> &newSolution, std::list<std::vector<float>> &compareSol, int counter, int startValue)
{
  bool newSolutionIsGreater = false;
  
  bool dominated = false;
  
  if(!pruningValues_.empty())
  {
    if(!isValidAccordingToPruning(newSolution, counter, startValue))
    {
      return;
    }
  }
  
  if(compareSol.empty())
  {
    solutions_.push_back(newSolution);
  
    compareSol.push_back(newSolution);
    
    return;
  }
  
  for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
  {
    if(!newSolutionIsGreater and !dlex(*sol, newSolution))
    {
      newSolutionIsGreater = true;
      
      solutions_.push_back(newSolution);
      
      sol = compareSol.insert(sol, newSolution);
      
      ++sol;
    }
    
    if(!newSolutionIsGreater and dominates(*sol, newSolution, true))
    {
      dominated = true;
      break;
    }
    
    if(newSolutionIsGreater)
    {
      if(dominates(newSolution, *sol, true))
      {
        sol = compareSol.erase(sol);
        --sol;
      }
    }
  }
  
  if(!dominated and !newSolutionIsGreater)
  {
    solutions_.push_back(newSolution);
  
    compareSol.push_back(newSolution);
  }
}

bool DP::isValidAccordingToPruning(std::vector<float> &sol, int counter, int startValue)
{
  bool weightIsGreater = false;
  
  for (auto pruneSol = pruningValues_[elements_.size() - counter]->rbegin() + startValue; pruneSol != pruningValues_[elements_.size() - counter]->rend(); ++pruneSol)
  {
    if(!weightIsGreater and sol.front() > pruneSol->weight_)
    {
      ++startValue;
      continue;
    }
    else
    {
      weightIsGreater = true;
    }
    
    if(sol.front() >= pruneSol->lowCapacity_)
    {
      bool validInRound = true;
      for (int i = 0; i < functionsRestricted_.size(); ++i)
      {
        if (sol[functionsRestricted_[i]] < pruneSol->solutionValues_.at(i + 1))
        {
          validInRound = false;
          break;
        }
      }
      if (validInRound)
      {
        return true;
      }
    }
  }
  return false;
}

bool DP::dominates(std::vector<float> &sol1, std::vector<float> &sol2, bool lastElement)
{
  if(!lastElement and sol1[0] > sol2[0])
  {
    return false;
  }
  
  for (int i = 1; i <= numberOfFunctions_; ++i)
  {
    if(sol1[i] < sol2[i])
    {
      return false;
    }
  }
  
  return true;
}

bool DP::lex(std::vector<float> &sol1, std::vector<float> &sol2)
{
  if (sol1[0] < sol2[0])
  {
    return true;
  }
  
  return sol1[0] == sol2[0] and dlex(sol1, sol2);
}

bool DP::dlex(std::vector<float> &sol1, std::vector<float> &sol2)
{
  for (int i = 1; i <= numberOfFunctions_; ++i)
  {
    if (sol1[i] != sol2[i])
    {
      return sol1[i] > sol2[i];
    }
  }
  return true;
}

const std::vector<std::vector<float>> &DP::getSolutions() const
{
  return solutions_;
}

const std::list<std::vector<float>> &DP::getFinalSol() const
{
  return finalSol;
}
