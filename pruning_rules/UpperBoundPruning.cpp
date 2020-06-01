//
// Created by rick on 27.05.20.
//

#include <cmath>
#include <algorithm>
#include "UpperBoundPruning.h"
#include "../SolutionComparer.h"


UpperBoundPruning::UpperBoundPruning(int numberOfElements, std::vector<int> functionsToCompare, int capacity,
                                     ElementManager &elementManager):
                                     Pruning(numberOfElements),
                                     elementManager_(elementManager),
                                     functionsToCompare_(functionsToCompare),
                                      numberOfFunctions_(functionsToCompare.size()),
                                      capacity_(capacity),
                                      paretoPruning_(numberOfElements, functionsToCompare.size())
{
}

void UpperBoundPruning::setupForNextElement(std::list<static_vector>& currentBestSolutions)
{
  ++numberOfCurrentElement_;
  
  //! sort remaining elements
  std::vector<ElementWithOrderValue> maxOrder;
  
  maxOrder.reserve(elementManager_.getNumberOfElements() - numberOfCurrentElement_);
  
  std::vector<ElementWithOrderValue> sumOrder = maxOrder;//todo: Element with value should use direct value and not pointer
  
  elementManager_.removeElementFromOrder(numberOfCurrentElement_);
  
  for(auto ele = elementManager_.getElements().begin() + numberOfCurrentElement_; ele != elementManager_.getElements().end(); ++ele)
  {
    int sum = 0;
    
    for (auto h : functionsToCompare_)
    {
      sum += ele->posOrderValueWeightRatio_[h - 1];
    }
    
    sumOrder.emplace_back( &(*ele), sum);
    
    float val = 0;
    
    for (auto h : functionsToCompare_)
    {
      if(val < ele->posOrderValueWeightRatio_[h - 1])
      {
        val = ele->posOrderValueWeightRatio_[h - 1];
      }
    }
    
    val += (float) sum / (float)( elementManager_.getNumberOfElements() * numberOfFunctions_);
    
    maxOrder.emplace_back(&(*ele), val);
  }
  
  std::sort(sumOrder.begin(), sumOrder.end());
  
  std::sort(maxOrder.begin(), maxOrder.end());
  
  //!generate set F from paper
  greedyCompletionOfCurrentBestSolution_.clear();
  
  paretoPruning_.setupForNextElement(&greedyCompletionOfCurrentBestSolution_);
  
  for (auto sol: currentBestSolutions)
  {
    for (auto &ele : sumOrder)
    {
      if(sol.front() + ele.ele_->weight_ <= capacity_)
      {
        sol.front() += ele.ele_->weight_;
        
        for (int idx = 1; idx <= numberOfFunctions_; ++idx)
        {
          sol[idx] += ele.ele_->values_[functionsToCompare_[idx - 1] - 1];
        }
      }
    }
    if(!paretoPruning_.shouldSolutionBeRemoved(sol))
    {
      paretoPruning_.solutionWasAdded(sol);
    }
  }
  
  for (auto sol: currentBestSolutions)
  {
    for (auto &ele : maxOrder)
    {
      if (sol.front() + ele.ele_->weight_ <= capacity_)
      {
        sol.front() += ele.ele_->weight_;
      
        for (int idx = 1; idx <= numberOfFunctions_; ++idx)
        {
          sol[idx] += ele.ele_->values_[functionsToCompare_[idx - 1] - 1];
        }
      }
    }
    if (!paretoPruning_.shouldSolutionBeRemoved(sol))
    {
      paretoPruning_.solutionWasAdded(sol);
    }
  }
}

bool UpperBoundPruning::shouldSolutionBeRemoved(static_vector &solution)
{
  //!calculate number of solutions that should be removed
  static_vector upperBoundOfSol(upperBound(solution));
  
  bool rval = false;
  
  for (auto &compSol : greedyCompletionOfCurrentBestSolution_)
  {
    if (solutionComparer::dlex(compSol, upperBoundOfSol))
    {
      if(solutionComparer::dominates(compSol, upperBoundOfSol))
      {
        if(!std::equal(compSol.begin() + 1, compSol.end(), upperBoundOfSol.begin() + 1))
        {
          rval = true;
          break;
        }
      }
    }
    else
    {
      break;
    }
  }
  
  updateRemovedSolutions(rval);
  
  return rval;
}

static_vector UpperBoundPruning::upperBound(static_vector &sol)
{
  static_vector rval = sol;
  
  for (int pos = 1; pos <= functionsToCompare_.size(); ++pos)
  {
    int idxAccess = functionsToCompare_[pos - 1];
    
    auto& elementsOrdered = elementManager_.getOrderedRawElementsValue()[idxAccess - 1];
    
    bool capacityWasReached = false;
    
    int remainingCapacity = capacity_ - sol.front();
    
    int i = -1; //c_i reflects in for loop the last Element added
    
    int numberOfRemainingElements = elementsOrdered.size();
    
    for(auto &ele: elementsOrdered)
    {
      remainingCapacity -= ele.front();
      
      if(remainingCapacity < 0)
      {
        remainingCapacity += ele.front();
        
        capacityWasReached = true;
        
        ++i;
        
        break;
      }
      
      rval[pos] += ele[idxAccess];
      
      ++i;
    }
    
    if (!capacityWasReached)
    {
      continue;
    }
    
    if(i != numberOfRemainingElements - 1 and i != 0)
    {
      int a = std::floor(remainingCapacity * elementsOrdered[i + 1][idxAccess] / elementsOrdered[i + 1].front());//todo: divison have already been done, can take direct value
      
      int b = std::floor(elementsOrdered[i][idxAccess] - (elementsOrdered[i].front() - remainingCapacity) * elementsOrdered[i - 1][idxAccess]/elementsOrdered[i - 1].front());
      
      int max = a > b ? a : b;
      
      rval[pos] += max;
    }
    else
    {//! if last Element doesn't fit, then use upper bound by adding residual capacity of current Element
      rval[pos] += std::floor(remainingCapacity * elementsOrdered[i][idxAccess]/elementsOrdered[i].front());//todo: divison have already been done, can take direct value
    }
  }
  
  return rval;
}
