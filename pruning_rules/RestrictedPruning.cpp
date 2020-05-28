//
// Created by rick on 27.05.20.
//

#include "RestrictedPruning.h"

RestrictedPruning::RestrictedPruning(int numberOfElements, std::vector<std::vector<PruningSolution>> &thresholdValues,
                                     std::vector<int> functionsRestricted):
    Pruning(numberOfElements),
    thresholdValues_(std::move(thresholdValues)),
    functionsRestricted_(std::move(functionsRestricted)),
    firstSolutionSatisfyingWeight_(0)
{
  numberOfValidRounds_.push_back(0);
}

bool RestrictedPruning::shouldSolutionBeRemoved(static_vector &solution, int posOfOldSolution)
{
  bool rval = true;
  
  if(posOfOldSolution != -1)
  {
    if(prevNumberOfValidRounds_[posOfOldSolution] > 1)
    {
      validRoundsOfCurrentSolution_ = prevNumberOfValidRounds_[posOfOldSolution] - 1;
      
      return false;
    }
  }
  
  bool weightIsGreater = false;
  
  for (auto pruneSol = thresholdValues_[thresholdValues_.size() - numberOfCurrentElement_].rbegin() + firstSolutionSatisfyingWeight_; pruneSol != thresholdValues_[thresholdValues_.size() - numberOfCurrentElement_].rend(); ++pruneSol)
  {
    if(!weightIsGreater and solution.front() > pruneSol->weight_)
    {
      ++firstSolutionSatisfyingWeight_;
      continue;
    }
    else
    {
      weightIsGreater = true;
    }
    
    if(solution.front() >= pruneSol->lowCapacity_)
    {
      bool satisfiesMinimalRequirement = true;
      
      for (int i = 0; i < functionsRestricted_.size(); ++i)
      {
        if (solution[functionsRestricted_[i]] < pruneSol->solutionValues_.at(i))
        {
          satisfiesMinimalRequirement = false;
          break;
        }
      }
      
      if (satisfiesMinimalRequirement)
      {
        validRoundsOfCurrentSolution_ = pruneSol->relevantRounds_;
        
        rval = false;
        
        break;
      }
    }
  }
  
  updateRemovedSolutions(rval);
  
  return rval;
}


void RestrictedPruning::setupForNextElement()
{
  ++numberOfCurrentElement_;
  
  firstSolutionSatisfyingWeight_ = 0;
  
  prevNumberOfValidRounds_ = std::move(numberOfValidRounds_);
}

void RestrictedPruning::solutionWasAdded()
{
  numberOfValidRounds_.push_back(validRoundsOfCurrentSolution_);
}

void RestrictedPruning::updateNumberOfValidRounds_(int numberOfSolutionsToRemove)
{
  numberOfValidRounds_.erase(numberOfValidRounds_.begin(), numberOfValidRounds_.begin() + numberOfSolutionsToRemove);
}


