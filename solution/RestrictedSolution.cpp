//
// Created by rick on 04.05.20.
//

#include <chrono>
#include <cmath>
#include "RestrictedSolution.h"
#include "../DP/NormalDP.h"
#include "../DP/RestrictedDP.h"

void RestrictedSolution::generateSolutin()
{
  //! restricted DP and data extraction
  auto t1 = std::chrono::high_resolution_clock::now();
  
   minimalFunctionValues_ = std::vector<int>(problem_.getRestrictedFunctions().size(), 0);
  
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
    minimalFunctionValues_[i] = std::floor(minimalFunctionValues_[i] - problem_.getSlack()[i]);
  }

  //! rev DP
  revDP rDP(problem_, minimalFunctionValues_);
  
  rDP.run();
  
  //! final DP
  RestrictedDP finalDP(problem_, rDP.getPruningValues());
  
  finalDP.run();
  
  auto t2 = std::chrono::high_resolution_clock::now();
  
  runtime_ = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  
  solutions_ = finalDP.getSolutions();
  
  numberOfRemovedSolutionsPerRound_ = finalDP.getNumberOfRemovedSolutionsPerRound();
  
}

const std::vector<int> &RestrictedSolution::getMinimalFunctionValues() const
{
  return minimalFunctionValues_;
}
