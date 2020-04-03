//
// Created by rick on 13.03.20.
//

#ifndef KNAPSACK_PROBLEM_H
#define KNAPSACK_PROBLEM_H

#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>

struct elementsWithOrder
{
  std::vector<float> element_;
  
  std::vector<int> posAccordingToOrder_;
  
  std::vector<float> valueForOrder_;
  
  explicit elementsWithOrder(std::vector<float> element):
  element_(std::move(element))
  {
    posAccordingToOrder_.resize(element_.size() - 1);
  
    valueForOrder_.reserve(element_.size() - 1);
    
    for (int i = 1; i < element_.size(); ++i)
    {
      valueForOrder_.push_back(element_[i] / element_[0]);
    }
  }
  
  int sumOfPosOrder()
  {
    int rval = 0;
    
    for (auto val: posAccordingToOrder_)
    {
      rval += val;
    }
    return rval;
  }
};

struct elementWithValue
{
  elementsWithOrder* element;
  float value;
  
  elementWithValue( elementsWithOrder* ele, float val):
  element(ele),
  value(val)
  {}
  
  bool operator < (const elementWithValue& str) const
  {
    return (value < str.value);
  }
};

class problem
{
private:
  
  float capacity_;
  
  int numberOfFunctions_;
  
  float sumOfWeights_;

private:
  
  std::vector<int> restrictedFunctions_;
  
  std::vector<float> slack_;
  
  std::vector<elementsWithOrder> elementsWithInformation_;
  
  std::vector<std::vector<float>> elements_;
  
public:
  float getCapacity() const;
  
  int getNumberOfFunctions() const;
  
  float getSumOfWeights() const;
  
  const std::vector<int> &getRestrictedFunctions() const;
  
  const std::vector<std::vector<float>> &getElements() const;

  const std::vector<float> &getSlack() const;

private:
  
  void readData(std::string& filename);
  
  void initialiaze(std::string& filename);

public:
  problem(std::string filename, float capacity, int numberOfFunctions,  std::vector<int> restrictedFunctions, std::vector<float> slack);
  
  problem(std::string filename, int numberOfFunctions, std::vector<int> restrictedFunctions, std::vector<float> slack);
  
  void orderInformation();
  
  void reverseElements();
  
  void makeSumOrder();
  
  void makeMaxOrder();
  
  void makeMinOrder();
};


#endif //KNAPSACK_PROBLEM_H
