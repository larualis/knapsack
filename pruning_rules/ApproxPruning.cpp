//
// Created by rick on 01.06.20.
//

#include "ApproxPruning.h"

#include "../SolutionComparer.h"

ApproxPruning::ApproxPruning(int numberOfElements, int numberOfFunctions, int frequency, double error):
    Pruning(numberOfElements),
    currentBestSolutions_(nullptr),
    frequency_(frequency),
    numberOfElements_(numberOfElements),
    error_(error)
{
  numberOfRoundsWhereErrorWillBeUsed_ = std::floor(numberOfElements/frequency);
  
  sumOfErrorsNotUsed_ = 0;
}

bool ApproxPruning::shouldSolutionBeRemoved(static_vector& solution)
{
  bool rval = false;
  
  if(currentBestSolutions_->empty())
  {
    return false;
  }
  
  for (auto & sol : *currentBestSolutions_)
  {
    if(dominatesWithError(sol, solution))
    {
      rval = true;
      
      break;
    }
  }
  
  updateRemovedSolutions(rval);
  
  return rval;
}

void ApproxPruning::updateListOfBestSolutions(static_vector& solution)
{
  
  for(auto sol = currentBestSolutions_->begin() ;sol != currentBestSolutions_->end(); ++sol)
  {
    if(dominatesWithError(solution, *sol))
    {
      sol = currentBestSolutions_->erase(sol);
      
      --sol;
    }
  }
  
  currentBestSolutions_->push_back(solution);
  
}

void ApproxPruning::setupForNextElement(std::list<static_vector>* currentBestSolutions, int numberOfCurrentElement)
{
  currentBestSolutions_ = currentBestSolutions;
  
  double additionalError = sumOfErrorsNotUsed_ / numberOfRoundsWhereErrorWillBeUsed_;
  
  errorThisRound_ = 2 * ((double) numberOfCurrentElement/frequency_) /((std::floor(numberOfElements_/frequency_) + 1) * (std::floor(numberOfElements_/frequency_))) + additionalError;
  
  errorCalculatedForDomination_ = errorThisRound_ * log2(1 + error_);
  
  sumOfErrorsNotUsed_ -=  additionalError;
  
  --numberOfRoundsWhereErrorWillBeUsed_;
  
  numberOfCurrentElement_ = numberOfCurrentElement;
}

void ApproxPruning::errorWasNotUsed()
{
  sumOfErrorsNotUsed_ += errorThisRound_;
}
