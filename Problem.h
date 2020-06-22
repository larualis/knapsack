//
// Created by rick on 13.03.20.
//

#ifndef KNAPSACK_PROBLEM_H
#define KNAPSACK_PROBLEM_H

#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "ElementManager.h"

class Problem
{
private:
  
  int capacity_;
  
  int numberOfFunctions_;
  
  int sumOfWeights_;
  
  std::vector<int> restrictedFunctions_;
  
  std::vector<float> slack_;
  
  double error_;

  ElementManager eleManager_;
public:
  ElementManager &getEleManager();

  int getCapacity() const;
  
  int getNumberOfFunctions() const;
  
  int getSumOfWeights() const;
  
  const std::vector<int> &getRestrictedFunctions() const;
  
  const std::vector<float> &getSlack() const;
  
  int getNumberOfElements();
  
  std::vector<int> getVectorOfAllFunctions_();
  
  double getError() const;

private:
  
  void readData(std::string& filename, std::vector<std::vector<int>>& rawElements);
  
public:
  Problem(std::string filename, std::vector<int> restrictedFunctions, std::vector<float> slack , double error);
  
  void reverseElements();
  
  void makeSumOrder(std::vector<int> functionsForOrder = std::vector<int>());
  
  void makeMaxOrder(std::vector<int> functionsForOrder = std::vector<int>());
  
  void makeMinOrder(std::vector<int> functionsForOrder = std::vector<int>());
};


#endif //KNAPSACK_PROBLEM_H
