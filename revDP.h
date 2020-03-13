//
// Created by rick on 12.03.20.
//

#ifndef KNAPSACK_REVDP_H
#define KNAPSACK_REVDP_H

#import <vector>

class revDP
{

private:
  const std::vector<std::vector<float>>& elements_;
  
  float capacity_;
  
  int numberOfFunctions_;
  
  std::vector<int> functionSubset_;
  
  std::vector<float> baseValues_;
  
  
  
public:
  const std::vector<std::vector<float>> &getSolutions() const;

public:
  revDP(const std::vector<std::vector<float>>& elements, float capacity ,int numberOfFunctions);
  
  revDP(const std::vector<std::vector<float>> &Elements, float capacity, int numberOfFunctions, std::vector<int> functionSubset);
  
  void run();

private:
  
  bool dominates(std::vector<float>& sol1, std::vector<float>& sol2, bool lastElement);
  
  
};


#endif //KNAPSACK_REVDP_H
