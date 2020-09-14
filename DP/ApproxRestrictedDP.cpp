//
// Created by rick on 04.06.20.
//

#include <iostream>
#include "ApproxRestrictedDP.h"
#include "../SolutionComparer.h"

ApproxRestrictedDP::ApproxRestrictedDP(Problem &problem, std::vector<int> functionsToCompare,
                                        int frequency, std::vector<std::vector<PruningSolution>>& pruningValues):
  problem_(problem),
  elementManager_(problem.getEleManager()),
  functionsToCompare_(functionsToCompare),
  numberOfFunctions_(functionsToCompare.size()),
  capacity_(problem.getCapacity()),
  numberOfCurrentElement_(0),
  remainingWeightPruning_(problem.getNumberOfElements(), problem.getSumOfWeights(), problem.getCapacity()),
  paretoPruner_(problem.getNumberOfElements(), functionsToCompare.size()),
  upperBoundPruning_(problem.getNumberOfElements(), functionsToCompare, problem.getCapacity(), problem.getEleManager()),
  restrictedPruning_(problem.getNumberOfElements() ,pruningValues, problem.getRestrictedFunctions()),
  approxPruning_(problem.getNumberOfElements(), problem.getRestrictedFunctions(), functionsToCompare.size(), frequency, problem_.getError()),
  frequency_(frequency)
{
  solutions_.emplace_back(static_vector(numberOfFunctions_ + 1,0));
}

void ApproxRestrictedDP::run()
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
    
    restrictedPruning_.setupForNextElement();
    
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
  
      restrictedPruning_.updateNumberOfValidRounds_(numberOfSolutionsToRemove);
    }
  }
}

void ApproxRestrictedDP::normalRound(const Element& element, std::list<static_vector>& currentBestSolutions)
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
      handleNewSolution(previousSolutions[idxOldSolution], idxOldSolution);
      
      ++idxOldSolution;
    }
    
    handleNewSolution(newSolution, -1);
    
    ++idxNewSolution;
  }
  
  while(idxOldSolution < previousSolutions.size())
  {
    handleNewSolution(previousSolutions[idxOldSolution], idxOldSolution);
    
    ++idxOldSolution;
  }
}

void ApproxRestrictedDP::handleNewSolution(static_vector &solution, int idxOldSolution)
{
  if(!paretoPruner_.shouldSolutionBeRemoved(solution) and !restrictedPruning_.shouldSolutionBeRemoved(solution, idxOldSolution))
  {
    solutions_.push_back(solution);
  
    restrictedPruning_.solutionWasAdded();
  
    paretoPruner_.solutionWasAdded(solution);
  }
}

void ApproxRestrictedDP::errorRound(const Element& element,std::list<static_vector>& currentBestSolutions)
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
      handleNewSolutionWithError(previousSolutions[idxOldSolution], errorWasUsed, idxOldSolution);
      
      ++idxOldSolution;
    }
    
    handleNewSolutionWithError(newSolution, errorWasUsed, -1);
    
    ++idxNewSolution;
  }
  
  while(idxOldSolution < previousSolutions.size())
  {
    handleNewSolutionWithError(previousSolutions[idxOldSolution], errorWasUsed, idxOldSolution);
    
    ++idxOldSolution;
  }
  
  if(!errorWasUsed)
  {
    approxPruning_.errorWasNotUsed();
  }
}

void ApproxRestrictedDP::handleNewSolutionWithError(static_vector &solution, bool &errorWasUsed, int idxOldSolution)
{
  bool solutionGotDominated = false;
  
  bool gotDominatedParetoPruning = false;
  
  if(!solutionGotDominated)
  {
    solutionGotDominated = restrictedPruning_.shouldSolutionBeRemoved(solution, idxOldSolution);
  }
  
  if(!errorWasUsed and !solutionGotDominated)
  {
    gotDominatedParetoPruning = paretoPruner_.shouldSolutionBeRemoved(solution);
  
    solutionGotDominated = gotDominatedParetoPruning;
  }
  
  if(!solutionGotDominated and !gotDominatedParetoPruning)
  {
    if(approxPruning_.shouldSolutionBeRemoved(solution))
    {
      errorWasUsed = true;
  
      solutionGotDominated = true;
    }
    else
    {
      solutionGotDominated = false;
    }
  }
  
  if(!solutionGotDominated)
  {
    solutions_.push_back(solution);
  
    restrictedPruning_.solutionWasAdded();
  
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

const std::vector<static_vector> &ApproxRestrictedDP::getSolutions() const
{
  return solutions_;
}

std::vector<std::vector<int>> ApproxRestrictedDP::getNumberOfRemovedSolutionsPerRound()
{
  std::vector<std::vector<int>> rval;
  
  rval.push_back(remainingWeightPruning_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(paretoPruner_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(restrictedPruning_.getNumberOfRemovedSolutionsPerRound());
  
  rval.push_back(upperBoundPruning_.getNumberOfRemovedSolutionsPerRound());
  
  return rval;
}

