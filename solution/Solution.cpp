//
// Created by rick on 01.06.20.
//

#include <chrono>
#include <cmath>
#include "Solution.h"
#include "../DP/NormalDP.h"
#include "../revDP.h"
#include "../DP/RestrictedDP.h"
#include "../DP/ApproxDP.h"
#include "../DP/ApproxRestrictedDP.h"
#include "../DP/revDPSimple.h"

Solution::Solution(Problem &problem):
    problem_(problem),
    runtime_(0)
{
}

void Solution::makeNormalSolution()
{
  NormalDP dp(problem_, problem_.getVectorOfAllFunctions_());
  
  auto t1 = std::chrono::high_resolution_clock::now();
  dp.run();
  auto t2 = std::chrono::high_resolution_clock::now();
  
  solutions_ = dp.getSolutions();
  
  runtime_ = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  
  numberOfRemovedSolutionsPerRound_ = dp.getNumberOfRemovedSolutionsPerRound();
}

void Solution::makeApproxSolution()
{
  ApproxDP dp(problem_, problem_.getVectorOfAllFunctions_(), std::floor(log2(problem_.getNumberOfElements())));
  
  auto t1 = std::chrono::high_resolution_clock::now();
  dp.run();
  auto t2 = std::chrono::high_resolution_clock::now();
  
  
  solutions_ = dp.getSolutions();
  
  runtime_ = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  
  numberOfRemovedSolutionsPerRound_ = dp.getNumberOfRemovedSolutionsPerRound();
}

void Solution::makeRestrictedSolution()
{
  //! restricted DP and data extraction
  auto t1 = std::chrono::high_resolution_clock::now();
  
  std::vector<int> minimalFunctionValues_ = std::vector<int>(problem_.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < problem_.getRestrictedFunctions().size(); ++i)
  {
    NormalDP singleDP(problem_, std::vector<int> {problem_.getRestrictedFunctions()[i]});
    
    singleDP.run();
    
    auto solutions = singleDP.getSolutions();
    
    for (auto sol: solutions)
    {
      if(minimalFunctionValues_[i] < sol[1])
      {
        minimalFunctionValues_[i] = sol[1];
      }
    }
  }
  
  for (int i = 0; i < minimalFunctionValues_.size(); ++i )
  {
    minimalFunctionValues_[i] = std::floor(minimalFunctionValues_[i] * problem_.getSlack()[i]);
  }
  
  //! rev DP
  revDPSimple rDP(problem_, minimalFunctionValues_);
  
  rDP.run();
  
  //! final DP
  RestrictedDP finalDP(problem_, rDP.getPruningValues());
  
  finalDP.run();
  
  auto t2 = std::chrono::high_resolution_clock::now();
  
  runtime_ = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  
  solutions_ = finalDP.getSolutions();
  
  numberOfRemovedSolutionsPerRound_ = finalDP.getNumberOfRemovedSolutionsPerRound();
  
}

void Solution::makeApproxRestrictedSolution()
{
  //! restricted DP and data extraction
  auto t1 = std::chrono::high_resolution_clock::now();
  
  std::vector<int> minimalFunctionValues_ = std::vector<int>(problem_.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < problem_.getRestrictedFunctions().size(); ++i)
  {
    NormalDP singleDP(problem_, std::vector<int> {problem_.getRestrictedFunctions()[i]});
    
    singleDP.run();
    
    auto solutions = singleDP.getSolutions();
    
    for (auto sol: solutions)
    {
      if(minimalFunctionValues_[i] < sol[1])
      {
        minimalFunctionValues_[i] = sol[1];
      }
    }
  }
  
  for (int i = 0; i < minimalFunctionValues_.size(); ++i )
  {
    minimalFunctionValues_[i] = std::floor(minimalFunctionValues_[i] * problem_.getSlack()[i]);
  }
  
  //! rev DP
  revDPSimple rDP(problem_, minimalFunctionValues_);
  
  rDP.run();
  
  //! final DP
  ApproxRestrictedDP finalDP(problem_,problem_.getVectorOfAllFunctions_(), std::floor(log2(problem_.getNumberOfElements())), rDP.getPruningValues());
  
  finalDP.run();
  
  auto t2 = std::chrono::high_resolution_clock::now();
  
  runtime_ = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  
  solutions_ = finalDP.getSolutions();
  
  numberOfRemovedSolutionsPerRound_ = finalDP.getNumberOfRemovedSolutionsPerRound();
  
}