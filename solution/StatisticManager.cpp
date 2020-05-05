//
// Created by rick on 05.05.20.
//

#include <iostream>
#include "StatisticManager.h"

StatisticManager::StatisticManager()
{
  countKnapsack_ = 0;
}

void StatisticManager::addSolution(Solution& sol)
{
  ++countKnapsack_;
  
  runtime_.push_back(sol.getRuntime());
  
  solutionSize_.push_back(sol.getSolutionSize());
  
  solutions_.push_back(sol.getSolutions());
}

void StatisticManager::printStatistics(bool printValues)
{
  if(printValues or runtime_.size() == 1)
  {
    int count = 0;
    
    for (auto &solutions : solutions_)
    {
      ++count;
      
      std::cout <<"\n" << "Solution " << count <<"\n"<<"\n";
      
      for(auto &sol: solutions)
      {
        for (auto x : sol)
        {
          std::cout << x <<" ";
        }
        std::cout << "\n";
      }
    }
  }
  
  float avgRuntime = 0;
  
  float avgSolutionSize = 0;
  
  
  for(auto x : runtime_)
  {
    avgRuntime += x;
  }
  avgRuntime = avgRuntime / countKnapsack_;
  
  for(auto x : solutionSize_)
  {
    avgSolutionSize += x;
  }
  avgSolutionSize = avgSolutionSize / countKnapsack_;
  
  std::cout << "\n";
  
  std::cout << "solved number of knapsack problems: " << countKnapsack_<<"\n";
  
  std::cout << "average time: " << avgRuntime<<"\n";
  
  std::cout << "average number of solution: " << avgSolutionSize<<"\n";
}

void StatisticManager::printCompareToOtherSolutions(StatisticManager &otherManager, bool detailed)
{
  float compRuntime = 0;
  
  float compSolutionSize = 0;
  
  for (int i = 0; i < countKnapsack_; ++i)
  {
    compRuntime += (float) otherManager.getRuntime()[i] / runtime_[i];
    
    compSolutionSize += (float) solutionSize_[i] / otherManager.getSolutionSize()[i];
    
    if(detailed)
    {
      std::cout<< (float) otherManager.getRuntime()[i] / runtime_[i] <<" "<<(float) otherManager.getSolutionSize()[i] / solutionSize_[i];
    }
  }
  
  compRuntime = compRuntime / countKnapsack_;
  
  compSolutionSize = compSolutionSize / countKnapsack_;
  
  std::cout << "\n";
  
  std::cout << "solved number of knapsack problems: " << countKnapsack_<<"\n";
  
  std::cout << "average time factor: " << compRuntime<<"\n";
  
  std::cout << "average number of solution factor: " << compSolutionSize<<"\n";
}

const std::vector<int> &StatisticManager::getRuntime() const
{
  return runtime_;
}

const std::vector<int> &StatisticManager::getSolutionSize() const
{
  return solutionSize_;
}