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
  
  float capacity_;
  
  int numberOfFunctions_;
  
  float sumOfWeights_;
  
  std::vector<int> restrictedFunctions_;
  
  std::vector<float> slack_;
  
  ElementManager eleManager_;
public:
  ElementManager &getEleManager();

  float getCapacity() const;
  
  int getNumberOfFunctions() const;
  
  float getSumOfWeights() const;
  
  const std::vector<int> &getRestrictedFunctions() const;
  
  const std::vector<float> &getSlack() const;

private:
  
  void readData(std::string& filename, std::vector<std::vector<float>>& rawElements);
  
public:
  Problem(std::string filename, float capacity, std::vector<int> restrictedFunctions, std::vector<float> slack);
  
  Problem(std::string filename, std::vector<int> restrictedFunctions, std::vector<float> slack);
  
  void reverseElements();
  
  void makeSumOrder();
  
  void makeMaxOrder();
  
  void makeMinOrder();
};


#endif //KNAPSACK_PROBLEM_H
