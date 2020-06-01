//
// Created by rick on 05.05.20.
//

#include <iostream>
#include <fstream>
#include <numeric>
#include <sstream>
#include "StatisticManager.h"

StatisticManager::StatisticManager(std::string pathToFiles):
pathToFiles_(pathToFiles)
{
  countKnapsack_ = 0;
}

void StatisticManager::addSolution(Solution& sol)
{
  ++countKnapsack_;
  
  runtime_.push_back(sol.getRuntime());
  
  solutionSize_.push_back(sol.getSolutionSize());
  
  solutions_.push_back(sol.getSolutions());
  
  prunedPerRound_.push_back(sol.getNumberOfRemovedSolutionsPerRound());
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
  
  std::cout << "\n";
  
  std::cout << "solved number of knapsack problems: " << countKnapsack_<<"\n";
  
  std::cout << "average time: " << std::accumulate(runtime_.begin(), runtime_.end(), 0.0)/countKnapsack_<<"\n";
  
  std::cout << "time range from " << *std::min_element(runtime_.begin(), runtime_.end()) << " to " << *std::max_element(runtime_.begin(), runtime_.end())<<"\n";
  
  std::cout << "average number of solution: " << std::accumulate(solutionSize_.begin(), solutionSize_.end(), 0.0)/countKnapsack_<<"\n";
  
  std::cout << "size range from " << *std::min_element(solutionSize_.begin(), solutionSize_.end()) << " to " << *std::max_element(solutionSize_.begin(), solutionSize_.end())<<"\n";
  
  std::cout << "\n";
  
  printDetailedPruning();
}

void StatisticManager::printCompareToOtherSolutions(StatisticManager &otherManager, bool detailed)
{
  std::vector<float> comparisonRuntime;
  
  std::vector<float> comparisonSolutionSize;
  
  for (int i = 0; i < countKnapsack_; ++i)
  {
    comparisonSolutionSize.push_back((float) solutionSize_[i] / otherManager.getSolutionSize()[i]);
  
    comparisonRuntime.push_back((float) otherManager.getRuntime()[i] / runtime_[i]);
  }
  
  if(detailed)
  {
    for (int i = 0; i < countKnapsack_; ++i)
    {
      std::cout << comparisonRuntime[i] << "\t" << comparisonSolutionSize[i];
      std::cout<<"\n";
    }
  }
  
  std::cout << "\n";
  
  std::cout << "solved number of knapsack problems: " << countKnapsack_<<"\n";
  
  std::cout << "average time factor: " << std::accumulate(comparisonRuntime.begin(), comparisonRuntime.end(), 0.0) / countKnapsack_ << "\n";
  
  std::cout << "time range from " << *std::min_element(comparisonRuntime.begin(), comparisonRuntime.end()) << " to " << *std::max_element(comparisonRuntime.begin(), comparisonRuntime.end()) << "\n";
  
  std::cout << "average number of solution factor: " << std::accumulate(comparisonSolutionSize.begin(), comparisonSolutionSize.end(), 0.0) / countKnapsack_ << "\n";
  
  std::cout << "size range from " << *std::min_element(comparisonSolutionSize.begin(), comparisonSolutionSize.end()) << " to " << *std::max_element(comparisonSolutionSize.begin(), comparisonSolutionSize.end()) << "\n";
  
  std::cout << "\n";
}

void StatisticManager::writeStatistics()
{
  std::fstream outputFile;
  
  outputFile.open(pathToFiles_+"/results_normal.txt", std::fstream::out);
  
  outputFile << "solved number of knapsack problems: " << countKnapsack_<<"\n";
  
  outputFile << "average time: " << std::accumulate(runtime_.begin(), runtime_.end(), 0.0)/countKnapsack_<<"\n";
  
  outputFile << "time range from " << *std::min_element(runtime_.begin(), runtime_.end()) << " to " << *std::max_element(runtime_.begin(), runtime_.end())<<"\n";
  
  outputFile << "average number of solution: " << std::accumulate(solutionSize_.begin(), solutionSize_.end(), 0.0)/countKnapsack_<<"\n";
  
  outputFile << "size range from " << *std::min_element(solutionSize_.begin(), solutionSize_.end()) << " to " << *std::max_element(solutionSize_.begin(), solutionSize_.end())<<"\n";
  
  outputFile << "\n";
  
  outputFile << "values: size time" << "\n";
  
  for(int i = 0; i < countKnapsack_ - 1; ++i)
  {
    outputFile << solutionSize_[i] << "\t" << runtime_[i] << "\n";
  }
  outputFile << solutionSize_.back() << "\t" << runtime_.back();
  
  outputFile.close();
}

void StatisticManager::readFromFile(std::string pathToFile)
{
  std::fstream file;
  
  file.open(pathToFile);
  
  std::string line;
  
  bool startReading = false;
  
  while(!startReading)
  {
    std::getline(file, line);
    
    startReading = line.find("values") != std::string::npos;
  }
  
  int val;
  
  while(std::getline(file, line))
  {
    std::stringstream ss(line);
  
    ss >> val;
  
    solutionSize_.push_back(val);
  
    ss >> val;
  
    runtime_.push_back(val);
  
    ++countKnapsack_;
  }
  file.close();
}

const std::vector<int> &StatisticManager::getRuntime() const
{
  return runtime_;
}

const std::vector<int> &StatisticManager::getSolutionSize() const
{
  return solutionSize_;
}

void StatisticManager::printDetailedPruning()
{
  
  int counter = 0;
  for(auto& prunedPerRoundThisElement : prunedPerRound_)
  {
    ++counter;
    
    std::cout<< "removed in Knapsack " << counter << "\n";
    
    int counter2 = 0;
    for(auto& prunedPerRoundByRule: prunedPerRoundThisElement)
    {
      ++counter2;
      std::cout<< "removed by Rule " << counter2 << "\n";
      for(auto numberRemovedThisRound: prunedPerRoundByRule)
      {
        std::cout<< numberRemovedThisRound <<" ";
      }
      std::cout<<"\n";
    }
  }
}
