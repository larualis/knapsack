//
// Created by rick on 11.03.20.
//

#include "DP.h"
#include "problem.h"

DP::DP(const problem &Problem, std::vector<std::vector<std::vector<float>>*> pruningValues):
    elements_(Problem.getElements()),
    capacity_(Problem.getCapacity()),
    numberOfFunctions_(Problem.getNumberOfFunctions()),
    functionsRestricted_(Problem.getRestrictedFunctions()),
    pruningValues_(std::move(pruningValues))
{
  solutions_.emplace_back(std::vector<float> (numberOfFunctions_ + 1, 0));
  functionsToCompare_.reserve(numberOfFunctions_);
  for(int i = 1; i <= numberOfFunctions_; ++i)
  {
    functionsToCompare_.push_back(i);
  }
}

DP::DP(const problem &Problem, std::vector<int>& functionsToCompare):
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
  //! adds element to all allowed
  for (auto element = elements_.begin(); element != elements_.end(); ++element)
  {
    int prevSolutionSize = solutions_.size();
    
    ++counter;
    
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
    
    bool lastElement = element == elements_.end() - 1;
    
    if(!pruningValues_.empty())
    {
      for (auto sol = solutions_.begin(); sol < solutions_.end(); ++sol)
      {
        if(!pruneDomination(*sol, counter))
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

bool DP::dominates(std::vector<float> &sol1, std::vector<float> &sol2, bool lastElement)
{
  if(sol1[0] > sol2[0] and !lastElement)
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

bool DP::pruneDomination(std::vector<float> &sol, int counter)
{
  for (auto pruneSol : *(pruningValues_[counter]))
  {
    if(sol.begin() <= pruneSol.begin() and sol.begin() >= pruneSol.end())
    {
      bool valid = true;
      for(auto i: functionsRestricted_)
      {
        if(valid and sol[i] > pruneSol[i])
        {
          valid = false;
        }
      }
      if (valid)
      {
        return true;
      }
    }
  }
  return false;
}

const std::vector<std::vector<float>> &DP::getSolutions() const
{
  return solutions_;
}
