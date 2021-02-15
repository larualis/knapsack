//
// Created by rick on 05.05.20.
//

#ifndef KNAPSACK_STATISTICMANAGER_H
#define KNAPSACK_STATISTICMANAGER_H


#include <vector>
#include <list>
#include "Solution.h"

class StatisticManager
{
private:
  std::vector<double> runtime_;
  
  std::vector<int> solutionSize_;
  
  std::list<std::vector<static_vector>> solutions_;
  
  std::vector<std::vector<std::vector<int>>> prunedPerRound_;
  
  float countKnapsack_;
  
  std::string pathToFiles_;
  
public:
  
  StatisticManager(std::string pathToFiles);
  
  void addSolution(Solution& sol);
  
  void printStatistics(bool printValues);
  
  void writeStatistics();
  
  void readFromFile(std::string pathToFile);
  
  const std::vector<int> &getSolutionSize() const;
  
  const std::vector<double> &getRuntime() const;
  
  void printCompareToOtherSolutions(StatisticManager& otherManager, bool detailed);
  
  void printDetailedPruning();
};


#endif //KNAPSACK_STATISTICMANAGER_H
