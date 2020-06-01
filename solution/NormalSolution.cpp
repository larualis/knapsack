//
// Created by rick on 04.05.20.
//

#include <chrono>
#include "NormalSolution.h"
#include "../DP/NormalDP.h"

void NormalSolution::generateSolutin()
{
  NormalDP dp(problem_, problem_.getVectorOfAllFunctions_());
  
  auto t1 = std::chrono::high_resolution_clock::now();
  dp.run();
  auto t2 = std::chrono::high_resolution_clock::now();
  
  
  solutions_ = dp.getSolutions();
  
  runtime_ = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  
  numberOfRemovedSolutionsPerRound_ = dp.getNumberOfRemovedSolutionsPerRound();
}