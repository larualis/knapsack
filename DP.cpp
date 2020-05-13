//
// Created by rick on 11.03.20.
//

#include <iostream>
#include "DP.h"
#include "Problem.h"
#include <list>
#include <cmath>

DP::DP(Problem& problem, std::vector<std::shared_ptr<std::vector<PruningSolution>>> pruningValues):
    problem_(problem),
    elementManager_(problem.getEleManager()),
    capacity_(problem.getCapacity()),
    numberOfFunctions_(problem.getNumberOfFunctions()),
    functionsRestricted_(problem.getRestrictedFunctions()),
    pruningValues_(std::move(pruningValues))
{
  solutions_.emplace_back(static_vector(numberOfFunctions_ + 1,0));
  
  validRounds_.push_back(0);
  
  for(int i = 1; i <= numberOfFunctions_; ++i)
  {
    functionsToCompare_.push_back(i);
  }
}

DP::DP(Problem &problem, std::vector<int> functionsToCompare):
    problem_(problem),
    elementManager_(problem.getEleManager()),
    capacity_(problem.getCapacity()),
    numberOfFunctions_(functionsToCompare.size()),
    functionsToCompare_(functionsToCompare)
{
  solutions_.emplace_back(static_vector(numberOfFunctions_ + 1,0));
  
  validRounds_.push_back(0);
}

void DP::run()
{
  int numberOfCurrentElement = 0;
  
  int weightOfRemainingElements = problem_.getSumOfWeights();
  
  for (const auto & element : elementManager_.getElements())
  {
   ++numberOfCurrentElement;
    
    std::cout << numberOfCurrentElement << "\r" << std::flush;
  
    //! i in paper
    int idxNewSolution = 0;
    
    //! j in paper
    int idxOldSolution = 0;
    
    std::vector<static_vector> previousSolutions = std::move(solutions_);
    
    std::list<static_vector> compareSol;
    
  
    std::vector<int> validPreviousRounds = std::move(validRounds_);
    
    int startValue = 0;//todo:rename
  
  
    while (idxOldSolution < previousSolutions.size() and previousSolutions[idxOldSolution][0] + weightOfRemainingElements <= capacity_)
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
      
      while (idxOldSolution < previousSolutions.size() and lex(previousSolutions[idxOldSolution], newSolution))
      {
        maintainNonDominated(previousSolutions[idxOldSolution], validPreviousRounds[idxOldSolution], compareSol, numberOfCurrentElement, startValue);
        
        ++idxOldSolution;
      }

      maintainNonDominated(newSolution, 0, compareSol, numberOfCurrentElement, startValue);
      
      ++idxNewSolution;
    }
    
    while(idxOldSolution < previousSolutions.size())
    {
      maintainNonDominated(previousSolutions[idxOldSolution], validPreviousRounds[idxOldSolution], compareSol, numberOfCurrentElement, startValue);
  
      ++idxOldSolution;
    }
  
    weightOfRemainingElements -= element.weight_;
    
    if(weightOfRemainingElements == 0)
    {
      std::vector finalSol (compareSol.begin(), compareSol.end());
      
      solutions_ = std::move(finalSol);
    }
    
    else
    {
      //!used for testing, used to disable last pruning rule
      if(false)
      {
        continue;
      }
      
      //! sort remaining elements
      std::vector<elementWithOrderValue> maxOrder;
  
      maxOrder.reserve(elementManager_.getNumberOfElements() - numberOfCurrentElement);
      
      std::vector<elementWithOrderValue> sumOrder = maxOrder;//todo: element with value should use direct value and not pointer
      
      elementManager_.removeElementFromOrder(numberOfCurrentElement);
      
      for(auto ele = elementManager_.getElements().begin() + numberOfCurrentElement; ele != elementManager_.getElements().end(); ++ele)
      {
        float sum = 0;
        
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
  
        val += sum / (float)( elementManager_.getNumberOfElements() * numberOfFunctions_);
        
        maxOrder.emplace_back(&(*ele), val);
      }
      
      std::sort(sumOrder.begin(), sumOrder.end());
      
      std::sort(maxOrder.begin(), maxOrder.end());
      
      //!generate set F from paper
      std::list<static_vector> compareSolF;
      
      for (auto sol: compareSol)
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
        keepNonDominated(sol, compareSolF);
      }
  
      for (auto sol: compareSol)
      {
        for (auto &ele : maxOrder)
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
        keepNonDominated(sol, compareSolF);
      }
      
      //!remove solutions according to upper bound rule
      int numberOfElementsToRemove = 0;
      
      for (auto &sol: solutions_)
      {
        static_vector upperBoundOfSol(upperBound(sol));
        
        bool remove = false;
        
        for (auto &compSol : compareSolF)
        {
          if (dlex(compSol, upperBoundOfSol))
          {
            if(dominates(compSol, upperBoundOfSol, true))
            {
              if(!std::equal(compSol.begin() + 1, compSol.end(), upperBoundOfSol.begin() + 1))
              {
                remove = true;
                break;
              }
            }
          }
          else
          {
           break;
          }
        }
        
        if(remove)
        {
          ++numberOfElementsToRemove;
        }
        else
        {
          break;
        }
      }
      
      solutions_.erase(solutions_.begin(), solutions_.begin() + numberOfElementsToRemove);
      
      validRounds_.erase(validRounds_.begin(), validRounds_.begin() + numberOfElementsToRemove);
    }
  }
}


void DP::maintainNonDominated(static_vector& solution, int validRound, std::list<static_vector> &compareSol, int counter, int& startValue)
{
  bool addSolution = true;
  
  bool solutionWasAddedToCompareSol = false;
  
  int validRoundsForNewSolution = 0;
  
  if(!pruningValues_.empty() and addSolution)
  {
    if (validRound < 1)
    {
      addSolution = isValidAccordingToPruning(solution, counter, startValue, validRoundsForNewSolution);
    }
    else
    {
      validRoundsForNewSolution = validRound - 1;
    }
  }
  
  if(!compareSol.empty() and addSolution)
  {
    for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
    {
      if(!solutionWasAddedToCompareSol and !dlex(*sol, solution))
      {
        solutionWasAddedToCompareSol = true;
        
        sol = compareSol.insert(sol, solution);
        
        ++sol;
      }
      
      if(!solutionWasAddedToCompareSol and dominates(*sol, solution, true))
      {
        addSolution = false;
        
        break;
      }
      
      if(solutionWasAddedToCompareSol)
      {
        if(dominates(solution, *sol, true))
        {
          sol = compareSol.erase(sol);
          --sol;
        }
      }
    }
  }
  
  if(addSolution)
  {
    solutions_.push_back(solution);
  
    validRounds_.push_back(validRoundsForNewSolution);
    
    if(!solutionWasAddedToCompareSol)
    {
      compareSol.push_back(solution);
    }
  }
}
//todo: should be rewritten according to maintainNonDoniated
void DP::keepNonDominated(static_vector &newSolution, std::list<static_vector> &compareSol)
{
  bool newSolutionIsGreater = false;
  
  bool dominated = false;
  
  if(compareSol.empty())
  {
    compareSol.push_back(newSolution);
    
    return;
  }
  
  for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
  {
    if(!newSolutionIsGreater and !dlex(*sol, newSolution))
    {
      newSolutionIsGreater = true;
      
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
    compareSol.push_back(newSolution);
  }
}

static_vector DP::upperBound(static_vector &sol)
{
  static_vector rval = sol;
  
  for (int pos = 1; pos <= functionsToCompare_.size(); ++pos)
  {
    int idxAccess = functionsToCompare_[pos - 1];
    
    auto& elementsOrdered = elementManager_.getOrderedRawElementsValue()[idxAccess - 1];
    
    bool capacityWasReached = false;
  
    int remainingCapacity = capacity_ - sol.front();
  
    int i = -1; //c_i reflects in for loop the last element added
  
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
    {//! if last element doesn't fit, then use upper bound by adding residual capacity of current element
      rval[pos] += std::floor(remainingCapacity * elementsOrdered[i][idxAccess]/elementsOrdered[i].front());//todo: divison have already been done, can take direct value
    }
  }
  
  return rval;
}

bool DP::isValidAccordingToPruning(static_vector &sol, int counter, int& startValue, int &validForRounds)
{
  bool weightIsGreater = false;
  
  //todo: experimental
  if(true and functionsRestricted_.size() == 1)
  {
    for (auto pruneSol = pruningValues_[elementManager_.getNumberOfElements() - counter]->rbegin() + startValue; pruneSol != pruningValues_[elementManager_.getNumberOfElements() - counter]->rend(); ++pruneSol)
    {
      if (pruneSol->weight_ >= sol.front())
      {
        if( pruneSol->solutionValues_.back() <= sol[functionsRestricted_[0]])
        {
          validForRounds = pruneSol->relevantRounds_;
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        ++startValue;
      }
    }
  }
  
  for (auto pruneSol = pruningValues_[elementManager_.getNumberOfElements() - counter]->rbegin() + startValue; pruneSol != pruningValues_[elementManager_.getNumberOfElements() - counter]->rend(); ++pruneSol)
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
        validForRounds = pruneSol->relevantRounds_;
        return true;
      }
    }
  }
  return false;
}

bool DP::dominates(static_vector &sol1, static_vector &sol2, bool lastElement)
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

bool DP::lex(static_vector &sol1, static_vector &sol2)
{
  if (sol1[0] < sol2[0])
  {
    return true;
  }
  
  return sol1[0] == sol2[0] and dlex(sol1, sol2);
}

bool DP::dlex(static_vector &sol1, static_vector &sol2)
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

const std::vector<static_vector>& DP::getSolutions() const
{
  return solutions_;
}