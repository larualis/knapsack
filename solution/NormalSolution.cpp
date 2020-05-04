//
// Created by rick on 04.05.20.
//

#include <chrono>
#include "NormalSolution.h"
#include "../DP.h"

void NormalSolution::generateSolutin()
{
  std::vector<int> functionsToCompare(problem_.getNumberOfFunctions(), 0);
  
  for (int i = 0; i < problem_.getNumberOfFunctions(); ++i)
  {
    functionsToCompare[i] = i + 1;
  }
  
  DP dp(problem_, functionsToCompare);
  
  auto t1 = std::chrono::high_resolution_clock::now();
  dp.run();
  auto t2 = std::chrono::high_resolution_clock::now();
  
  
  solutions_ = dp.getSolutions();
  
  runtime_ = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
}