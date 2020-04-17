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
  solutions_.emplace_back(std::vector<float> (numberOfFunctions_ + 1, 0));
  validRounds_.push_back(0);
  functionsToCompare_.reserve(numberOfFunctions_);
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
  solutions_.emplace_back(std::vector<float> (numberOfFunctions_ + 1, 0));
  validRounds_.push_back(0);
}

void DP::run()
{
  int counter = 0;
  
  float weightOfRemainingElements = problem_.getSumOfWeights();
  
  for (const auto & element : elementManager_.getElements())
  {
    counter++;
    
    std::cout<<counter<<"\n";
    
    int i = 0;
    
    int j = 0;
    
    int startValue = 0;
  
    std::vector<std::vector<float>> previousSolutions = std::move(solutions_);
    
    std::vector<int> validPreviousRounds = std::move(validRounds_);
    
    std::list<std::vector<float>> compareSol;
    
    while (j < previousSolutions.size() and previousSolutions[j][0] + weightOfRemainingElements <= capacity_)
    {
      ++j;
    }
    
    while (i < previousSolutions.size() and previousSolutions[i][0] + element.weight_ <= capacity_)
    {
      std::vector<float> newSolution(numberOfFunctions_ + 1, 0);
      
      newSolution[0] = previousSolutions[i][0] + element.weight_;
      
      for (int idx = 1; idx <= numberOfFunctions_; ++idx)
      {
        newSolution[idx] = previousSolutions[i][idx] + element.values_.at(functionsToCompare_[idx - 1] - 1);
      }
      
      while (j < previousSolutions.size() and lex(previousSolutions[j], newSolution))
      {
        maintainNonDominated(previousSolutions[j], validPreviousRounds[j], compareSol, counter, startValue);
        
        ++j;
      }

      maintainNonDominated(newSolution, 0, compareSol, counter, startValue);
      
      ++i;
    }
    
    while(j < previousSolutions.size())
    {
      maintainNonDominated(previousSolutions[j], validPreviousRounds[j], compareSol, counter, startValue);
  
      ++j;
    }
  
    weightOfRemainingElements -= element.weight_;
    
    if(weightOfRemainingElements == 0)
    {
      finalSol = std::move(compareSol);
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
  
      maxOrder.reserve(elementManager_.getElements().size() - counter);
      
      std::vector<elementWithOrderValue> sumOrder = maxOrder;//todo: element with value should use direct value and not pointer
      
      elementManager_.removeElementFromOrder(counter);
      
      for(auto ele = elementManager_.getElements().begin() + counter; ele != elementManager_.getElements().end(); ++ele)
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
  
        val += sum / (float)( elementManager_.getElements().size() * numberOfFunctions_);
        
        maxOrder.emplace_back(&(*ele), val);
      }
      
      std::sort(sumOrder.begin(), sumOrder.end());
      
      std::sort(maxOrder.begin(), maxOrder.end());
      
      //!generate set F from paper
      std::list<std::vector<float>> compareSolF;
      
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
        std::vector<float> upperBoundOfSol(upperBound(sol));
        
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
    }
  }
}


void DP::maintainNonDominated(std::vector<float> &newSolution, int validRound, std::list<std::vector<float>> &compareSol, int counter, int startValue)
{
  bool newSolutionIsGreater = false;
  
  bool dominated = false;
  
  int validRoundsForNewSolution = 0;
  
  if(!pruningValues_.empty())
  {
    if (validRound < 1)
    {
      if (!isValidAccordingToPruning(newSolution, counter, startValue, validRoundsForNewSolution))
      {
        return;
      }
    }
    else
    {
      validRoundsForNewSolution = validRound - 1;
    }
  }
  
  if(compareSol.empty())
  {
    solutions_.push_back(newSolution);
    
    validRounds_.push_back(validRoundsForNewSolution);
  
    compareSol.push_back(newSolution);
    
    return;
  }
  
  for (auto sol = compareSol.begin(); sol != compareSol.end(); ++sol)
  {
    if(!newSolutionIsGreater and !dlex(*sol, newSolution))
    {
      newSolutionIsGreater = true;
      
      solutions_.push_back(newSolution);
  
      validRounds_.push_back(validRoundsForNewSolution);
      
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
  
    validRounds_.push_back(validRoundsForNewSolution);
  
    compareSol.push_back(newSolution);
  }
}

void DP::keepNonDominated(std::vector<float> &newSolution, std::list<std::vector<float>> &compareSol)
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

std::vector<float> DP::upperBound(std::vector<float> &sol)
{
  std::vector<float> rval = sol;
  
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

bool DP::isValidAccordingToPruning(std::vector<float> &sol, int counter, int startValue, int &validForRounds)
{
  bool weightIsGreater = false;
  
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

const std::vector<std::vector<float>> &DP::getSolutions() const
{
  return solutions_;
}

const std::list<std::vector<float>> &DP::getFinalSol() const
{
  return finalSol;
}