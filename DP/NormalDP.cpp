//
// Created by rick on 27.05.20.
//

#include <iostream>
#include "NormalDP.h"
#include "../SolutionComparer.h"

NormalDP::NormalDP(Problem &problem, std::vector<int> functionsToCompare):
    problem_(problem),
    elementManager_(problem.getEleManager()),
    functionsToCompare_(functionsToCompare),
    numberOfFunctions_(functionsToCompare.size()),
    capacity_(problem.getCapacity()),
    numberOfCurrentElement_(0),
    remainingWeightPruning_(problem.getNumberOfElements(), problem.getSumOfWeights(), problem.getCapacity()),
    paretoPruner_(problem.getNumberOfElements(), functionsToCompare.size()),
    upperBoundPruning_(problem.getNumberOfElements(), functionsToCompare, problem.getCapacity(), problem.getEleManager())
{
  solutions_.emplace_back(static_vector(numberOfFunctions_ + 1,0));
}

void NormalDP::run()
{
  for (const auto & element : elementManager_.getElements())
  {
    ++numberOfCurrentElement_;
    
    std::cout << numberOfCurrentElement_ << "\r" << std::flush;
    
    //! i in paper
    int idxNewSolution = 0;
    
    //! j in paper
    int idxOldSolution = 0;
    
    std::vector<static_vector> previousSolutions = std::move(solutions_);
    
    std::list<static_vector> currentBestSolutions;
  
    if(numberOfCurrentElement_ != 1)
    {
      remainingWeightPruning_.setupForNextElement( elementManager_.getElements()[numberOfCurrentElement_ - 2].weight_);
    }
    
    paretoPruner_.setupForNextElement(&currentBestSolutions);
    
    while (idxOldSolution < previousSolutions.size() and
        remainingWeightPruning_.shouldSolutionBeRemoved(previousSolutions[idxOldSolution]))
    {
      ++idxOldSolution;
    }

    while (idxNewSolution < previousSolutions.size() and previousSolutions[idxNewSolution][0] + element.weight_ <= capacity_)
    {
      static_vector newSolution(numberOfFunctions_ + 1,0);
      
      newSolution[0] = previousSolutions[idxNewSolution][0] + element.weight_;
      
      for (int idx = 1; idx <= numberOfFunctions_; ++idx)
      {
        newSolution[idx] = previousSolutions[idxNewSolution][idx] + element.values_.at(functionsToCompare_[idx - 1] - 1);
      }
      
      while (idxOldSolution < previousSolutions.size() and solutionComparer::lex(previousSolutions[idxOldSolution], newSolution))
      {
        handleNewSolution(previousSolutions[idxOldSolution], currentBestSolutions);
        
        ++idxOldSolution;
      }
  
      handleNewSolution(newSolution, currentBestSolutions);
      
      ++idxNewSolution;
    }
    
    while(idxOldSolution < previousSolutions.size())
    {
      handleNewSolution(previousSolutions[idxOldSolution], currentBestSolutions);
      
      ++idxOldSolution;
    }
    
    if(numberOfCurrentElement_ == elementManager_.getNumberOfElements())
    {
      std::vector finalSol (currentBestSolutions.begin(), currentBestSolutions.end());
      
      solutions_ = std::move(finalSol);
    }
    else
    {
      //!used for testing, used to disable last pruning rule
      if(false)
      {
        continue;
      }
      
      int numberOfElementsToRemove = 0;
      
      upperBoundPruning_.setupForNextElement(currentBestSolutions);
      
      for(auto& sol: solutions_)
      {
        if(upperBoundPruning_.shouldSolutionBeRemoved(sol))
        {
          ++numberOfElementsToRemove;
        }
        else break;
      }
      
      solutions_.erase(solutions_.begin(), solutions_.begin() + numberOfElementsToRemove);
    }
  }
}

void NormalDP::handleNewSolution(static_vector &solution, std::list<static_vector> &currentBestSolutions)
{
  if(!paretoPruner_.shouldSolutionBeRemoved(solution))
  {
    solutions_.push_back(solution);
    
    paretoPruner_.solutionWasAdded(solution);
  }
}

const std::vector<static_vector> &NormalDP::getSolutions() const
{
  return solutions_;
}

std::vector<std::vector<int>> NormalDP::getNumberOfRemovedSolutionsPerRound()
{
  std::vector<std::vector<int>> rval;
  
  rval.push_back(remainingWeightPruning_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(paretoPruner_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(upperBoundPruning_.getNumberOfRemovedSolutionsPerRound());
  
  return rval;
}