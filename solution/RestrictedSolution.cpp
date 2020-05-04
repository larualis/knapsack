//
// Created by rick on 04.05.20.
//

#include <chrono>
#include <cmath>
#include <iostream>
#include "RestrictedSolution.h"
#include "../DP.h"

void RestrictedSolution::generateSolutin()
{
  //! restricted DP and data extraction
  auto t1 = std::chrono::high_resolution_clock::now();
  
   minimalFunctionValues_ = std::vector<float>(problem_.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < problem_.getRestrictedFunctions().size(); ++i)
  {
    DP singleDP(problem_, std::vector<int> {problem_.getRestrictedFunctions()[i]});
  
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
  std::cout<<"vector for calc"<<std::endl;
  
  //printVector<float>(minimalFunctionValues);
  
  //! rev DP
  revDP rDP(problem_, minimalFunctionValues_);
  
  auto pruningValues = rDP.run();
  
  //! final DP
  DP finalDP(problem_, pruningValues);
  
  finalDP.run();
  
  auto t2 = std::chrono::high_resolution_clock::now();
  
  runtime_ = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  
  solutions_ = finalDP.getSolutions();
  
}

const std::vector<float> &RestrictedSolution::getMinimalFunctionValues() const
{
  return minimalFunctionValues_;
}
