//
// Created by rick on 11.03.20.
//

#include <iostream>
#include "DP.h"
#include "problem.h"
#include <list>

DP::DP(const problem &Problem, std::vector<std::vector<std::vector<float>>*> pruningValues):
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
    
    int i = 0;
    
    int j = 0;
  
    std::vector<std::vector<float>> previousSolutions = std::move(solutions_);
    
    std::list<std::vector<float>> compareSol;
    
    while (j < previousSolutions.size() and previousSolutions[j][0] + weightOfRemainingElements <= capacity_)
    {
      ++j;
    }
    
    compareSol.push_back(previousSolutions[j]);
    
    solutions_.push_back(previousSolutions[j]);
    
    ++j;
    
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
        maintainNonDominated(previousSolutions[j], compareSol);
        
        ++j;
      }
      maintainNonDominated(newSolution, compareSol);
      
      ++i;
    }
    
    while(j < previousSolutions.size())
    {
      maintainNonDominated(previousSolutions[j], compareSol);
  
      ++j;
    }
  
    weightOfRemainingElements -= element.at(0);
    
    if(weightOfRemainingElements == 0)
    {
      finalSol = std::move(compareSol);
    }
  }
}
    /**
    int prevSolutionSize = solutions_.size();
    
    ++counter;
    
    std::cout <<counter<< std::endl;
    
    for (int sol = 0; sol < prevSolutionSize; ++sol)
    {
      if (solutions_[sol][0] + element->at(0) <= capacity_)
      {
        std::vector<float> newSolution(numberOfFunctions_ + 1, 0);
        
        newSolution[0] = solutions_[sol][0] + element->at(0);
  
        for(int i = 1; i <= numberOfFunctions_; ++i)
        {
          newSolution[i] = solutions_[sol][i] + element->at(functionsToCompare_[i - 1]);
        }
        
        solutions_.push_back(newSolution);
      }
    }
    
    bool lastElement = element == (elements_.end() - 1);
    
    //! rev pruning
    if(!pruningValues_.empty())
    {
      for (auto sol = solutions_.begin(); sol < solutions_.end(); ++sol)
      {
        if(!isValidAccordingToPruning(*sol, counter))
        {
          sol = solutions_.erase(sol);
          --sol;
        }
      }
    }
  
    //! simple pareto domination
    for (int pos = 0; pos < solutions_.size(); ++pos)
    {
      bool solDominated = false;
  
      for (auto compareSol = solutions_.begin(); compareSol != solutions_.begin() + pos; ++compareSol)
      {
        auto sol = solutions_.begin() + pos;
        
        if (!solDominated and dominates(*compareSol, *sol, lastElement))
        {
         solutions_.erase(sol);
         --pos;
         break;
        }
        
        if(dominates(*sol, *compareSol, lastElement))
        {
         compareSol = --solutions_.erase(compareSol);
         
         --pos;
         
         solDominated = true;
        }
      }
    }
  }
}
     **/

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

bool DP::isValidAccordingToPruning(std::vector<float> &sol, int counter)
{
  for (auto pruneSol : *(pruningValues_[elements_.size() - counter]))
  {
    if(sol.front() <= pruneSol.front() and sol.back() >= pruneSol.back())
    {
      bool validInRound = true;
      for (auto i: functionsRestricted_)
      {
        if (sol[i] < pruneSol[i])
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

void DP::maintainNonDominated(std::vector<float> &newSolution, std::list<std::vector<float>> &compareSol)
{
  bool newSolutionIsGreater = false;
  
  bool dominated = false;
  
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

const std::vector<std::vector<float>> &DP::getSolutions() const
{
  return solutions_;
}

const std::list<std::vector<float>> &DP::getFinalSol() const
{
  return finalSol;
}
