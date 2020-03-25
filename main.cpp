#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DP.h"
#include "problem.h"
#include "revDP.h"

template <class x>
void printVector(std::vector<x> vec)
{
  for (auto ele : vec)
  {
    std::cout << ele << " ";
  }
  std::cout << std::endl;
}

void advancedPruning(problem Problem)
{
  //! restricted DP and data extraction
  std::vector<float> minimalFunctionValues(Problem.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < Problem.getRestrictedFunctions().size(); ++i)
  {
    DP restrictedDP(Problem, std::vector<int> {Problem.getRestrictedFunctions()[i]});
  
    restrictedDP.run();
  
    auto solutions = restrictedDP.getSolutions();
  
    for (auto sol: solutions)
    {
      if(minimalFunctionValues[i] < sol[1])
      {
        minimalFunctionValues[i] = sol[1];
      }
    }
  }
  
  for (int i = 0; i < minimalFunctionValues.size(); ++i )
  {
    minimalFunctionValues[i] = minimalFunctionValues[i] - Problem.getSlack()[i];
  }
  std::cout<<"vector for calc"<<std::endl;
  
  printVector<float>(minimalFunctionValues);
  
  //! rev DP
  revDP rDP(Problem, minimalFunctionValues);
  
  auto pruningValues = rDP.run();
  
  //! final DP
  DP finalDP(Problem, pruningValues);
  
  finalDP.run();
  
  auto finaleSolution = finalDP.getSolutions();
  
  std::sort(finaleSolution.begin(), finaleSolution.end(), [](const std::vector<float> & a, const std::vector<float> & b){ return a[1] < b[1]; });
  
  for (auto sol: finaleSolution)
  {
    printVector<float>(sol);
  }
  
  for(auto & v : pruningValues)
  {
    v->clear();
    
    auto d = v;
    
    v = nullptr;
    
    delete d;
  }
}

void standartDP(problem Problem, std::vector<int> functionsToCompare)
{
  DP dp(Problem, functionsToCompare);
  
  dp.run();
  
  auto finaleSolution = dp.getSolutions();
  
  std::sort(finaleSolution.begin(), finaleSolution.end(), [](const std::vector<float> & a, const std::vector<float> & b){ return a[1] < b[1]; });
  
  for (auto sol: finaleSolution)
  {
    printVector<float>(sol);
  }
}

int main(int argc, char **argv)
{
  //!input
  std::vector<int> restrictedFunctions {1,2};
  
  std::vector<float> slack {200, 200};
  
  std::string data = "../python/knapsack.txt";
  
  problem Problem(data, 250, 3, restrictedFunctions, slack);
  
  advancedPruning(Problem);
  
  /**
  std::vector<int> functionsToCompare(Problem.getNumberOfFunctions(), 0);
  
  for (int i = 0; i < Problem.getNumberOfFunctions(); ++i)
  {
    functionsToCompare[i] = i + 1;
  }
  
  standartDP(Problem, functionsToCompare);
   **/
  
  return 0;
}

