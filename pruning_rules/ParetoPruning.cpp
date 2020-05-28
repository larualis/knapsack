//
// Created by rick on 27.05.20.
//

#include "ParetoPruning.h"

ParetoPruning::ParetoPruning(int numberOfElements, int numberOfFunctions):
    Pruning(numberOfElements),
    solComp_(numberOfFunctions),
    currentBestSolutions_(nullptr)
{

}

bool ParetoPruning::shouldSolutionBeRemoved(static_vector& solution)
{
  bool rval = false;
  
  auto sol = currentBestSolutions_->begin();
  
  if(currentBestSolutions_->empty())
  {
    placeToAddSolution_ = sol;
    
    return false;
  }
  
  for (; sol != currentBestSolutions_->end(); ++sol)
  {
    if(!solComp_.dlex(*sol, solution))
    {
      rval = false;
      
      break;
    }
    
    if(solComp_.dominates(*sol, solution))
    {
      rval = true;
      
      break;
    }
  }
  
  placeToAddSolution_ = sol;
  
  updateRemovedSolutions(rval);
  
  return rval;
}

void ParetoPruning::solutionWasAdded(static_vector& solution)
{
  auto sol = currentBestSolutions_->insert(placeToAddSolution_, solution);
  
  ++sol;
  
  for( ;sol != currentBestSolutions_->end(); ++sol)
  {
    if(solComp_.dominates(solution, *sol))
    {
      sol = currentBestSolutions_->erase(sol);
      
      --sol;
    }
  }
}

void ParetoPruning::setupForNextElement(std::list<static_vector>* currentBestSolutions)
{
  currentBestSolutions_ = currentBestSolutions;
  
  ++numberOfCurrentElement_;
}
