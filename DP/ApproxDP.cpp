//
// Created by rick on 01.06.20.
//

#include "ApproxDP.h"
#include <iostream>
#include "../SolutionComparer.h"

ApproxDP::ApproxDP(Problem &problem, std::vector<int> functionsToCompare, int frequency):
    problem_(problem),
    elementManager_(problem.getEleManager()),
    functionsToCompare_(functionsToCompare),
    numberOfFunctions_(functionsToCompare.size()),
    capacity_(problem.getCapacity()),
    numberOfCurrentElement_(0),
    remainingWeightPruning_(problem.getNumberOfElements(), problem.getSumOfWeights(), problem.getCapacity()),
    paretoPruner_(problem.getNumberOfElements(), functionsToCompare.size()),
    upperBoundPruning_(problem.getNumberOfElements(), functionsToCompare, problem.getCapacity(), problem.getEleManager()),
    approxPruning_(problem.getNumberOfElements(), functionsToCompare.size(), frequency),
    frequency_(frequency)
{
  solutions_.emplace_back(static_vector(numberOfFunctions_ + 1,0));
}

void ApproxDP::run()
{
  for (const auto & element : elementManager_.getElements())
  {
    ++numberOfCurrentElement_;
    
    std::cout << numberOfCurrentElement_ << "\r" << std::flush;
    
    std::list<static_vector> currentBestSolutions;
  
    if(numberOfCurrentElement_ != 1)
    {
      remainingWeightPruning_.setupForNextElement( elementManager_.getElements()[numberOfCurrentElement_ - 2].weight_);
    }
  
    paretoPruner_.setupForNextElement(&currentBestSolutions);
  
  
    if(numberOfCurrentElement_ % frequency_ == 0)
    {
      errorRound(element, currentBestSolutions);
    }
    else
    {
      normalRound(element, currentBestSolutions);
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
      
      int numberOfSolutionsToRemove = 0;
      
      upperBoundPruning_.setupForNextElement(currentBestSolutions);
      
      for(auto& sol: solutions_)
      {
        if(upperBoundPruning_.shouldSolutionBeRemoved(sol))
        {
          ++numberOfSolutionsToRemove;
        }
        else break;
      }
      
      solutions_.erase(solutions_.begin(), solutions_.begin() + numberOfSolutionsToRemove);
    }
  }
}

void ApproxDP::normalRound(const Element& element, std::list<static_vector>& currentBestSolutions)
{
  //! i in paper
  int idxNewSolution = 0;
  
  //! j in paper
  int idxOldSolution = 0;
  
  std::vector<static_vector> previousSolutions = std::move(solutions_);
  
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
      handleNewSolution(previousSolutions[idxOldSolution]);
      
      ++idxOldSolution;
    }
    
    handleNewSolution(newSolution);
    
    ++idxNewSolution;
  }
  
  while(idxOldSolution < previousSolutions.size())
  {
    handleNewSolution(previousSolutions[idxOldSolution]);
    
    ++idxOldSolution;
  }
}

void ApproxDP::handleNewSolution(static_vector &solution)
{
  if(!paretoPruner_.shouldSolutionBeRemoved(solution))
  {
    solutions_.push_back(solution);
    
    paretoPruner_.solutionWasAdded(solution);
  }
}

void ApproxDP::errorRound(const Element& element,std::list<static_vector>& currentBestSolutions)
{
  //! i in paper
  int idxNewSolution = 0;
  
  //! j in paper
  int idxOldSolution = 0;
  
  std::vector<static_vector> previousSolutions = std::move(solutions_);
  
  bool errorWasUsed = false;
  
  approxPruning_.setupForNextElement(&currentBestSolutions, numberOfCurrentElement_);
  
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
      handleNewSolutionWithError(previousSolutions[idxOldSolution], errorWasUsed);
      
      ++idxOldSolution;
    }
  
    handleNewSolutionWithError(newSolution, errorWasUsed);
    
    ++idxNewSolution;
  }
  
  while(idxOldSolution < previousSolutions.size())
  {
    handleNewSolutionWithError(previousSolutions[idxOldSolution], errorWasUsed);
    
    ++idxOldSolution;
  }
  
  if(!errorWasUsed)
  {
    approxPruning_.errorWasNotUsed();
  }
}

void ApproxDP::handleNewSolutionWithError(static_vector &solution, bool &errorWasUsed)
{
  bool solutionIsGood = false;
  
  if(!errorWasUsed)
  {
    solutionIsGood = !paretoPruner_.shouldSolutionBeRemoved(solution);
  }
  
  if(!solutionIsGood)
  {
    if(approxPruning_.shouldSolutionBeRemoved(solution))
    {
      errorWasUsed = true;
  
      solutionIsGood = false;
    }
    else
    {
      solutionIsGood = true;
    }
  }
  
  if(solutionIsGood)
  {
    solutions_.push_back(solution);
    
    if(errorWasUsed)
    {
      approxPruning_.updateListOfBestSolutions(solution);
    }
    else
    {
      paretoPruner_.solutionWasAdded(solution);
    }
  }
  
}

const std::vector<static_vector> &ApproxDP::getSolutions() const
{
  return solutions_;
}

std::vector<std::vector<int>> ApproxDP::getNumberOfRemovedSolutionsPerRound()
{
  std::vector<std::vector<int>> rval;
  
  rval.push_back(remainingWeightPruning_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(paretoPruner_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(upperBoundPruning_.getNumberOfRemovedSolutionsPerRound());
  
  return rval;
}
