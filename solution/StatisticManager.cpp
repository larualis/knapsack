//
// Created by rick on 05.05.20.
//

#include <iostream>
#include <numeric>
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
  
  std::cout << "\n";
}

void StatisticManager::printCompareToOtherSolutions(StatisticManager &otherManager, bool detailed)
{
  std::vector<float> compRuntime;
  
  std::vector<float> compSolutionSize;
  
  for (int i = 0; i < countKnapsack_; ++i)
  {
    compSolutionSize.push_back((float) solutionSize_[i] / otherManager.getSolutionSize()[i]);
  
    compRuntime.push_back((float) otherManager.getRuntime()[i] / runtime_[i]);
  }
  
  if(detailed)
  {
    for (int i = 0; i < countKnapsack_; ++i)
    {
      std::cout<< compRuntime[i] <<"\t"<<compSolutionSize[i];
      std::cout<<"\n";
    }
  }
  
  std::cout << "\n";
  
  std::cout << "solved number of knapsack problems: " << countKnapsack_<<"\n";
  
  std::cout << "average time factor: " << std::accumulate(compRuntime.begin(),compRuntime.end(),0.0)/countKnapsack_<<"\n";
  
  std::cout << "time range from " << *std::min_element(compRuntime.begin(),compRuntime.end())<<" to " << *std::max_element(compRuntime.begin(),compRuntime.end())<<"\n";
  
  std::cout << "average number of solution factor: " << std::accumulate(compSolutionSize.begin(),compSolutionSize.end(),0.0)/countKnapsack_<<"\n";
  
  std::cout << "size range from " << *std::min_element(compSolutionSize.begin(),compSolutionSize.end())<<" to " << *std::max_element(compSolutionSize.begin(),compSolutionSize.end())<<"\n";
  
  std::cout << "\n";
}

const std::vector<int> &StatisticManager::getRuntime() const
{
  return runtime_;
}

const std::vector<int> &StatisticManager::getSolutionSize() const
{
  return solutionSize_;
}