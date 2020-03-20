#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DP.h"
#include "problem.h"


int main()
{
  std::vector<int> restrictedFunctions {1,2};
  
  std::vector<float> slack {20, 20};
  
  std::string data = "../python/knapsack.txt";
  
  problem Problem(data, 150, 2, restrictedFunctions, slack);
  
  DP restrictedDP(Problem, restrictedFunctions);
  
  restrictedDP.run();
  
  auto solutions = restrictedDP.getSolutions();
  
  std::sort(solutions.begin(), solutions.end(), [](const std::vector<float> & a, const std::vector<float> & b){ return a[1] < b[1]; });
  
  for (auto sol: solutions)
  {
    for (auto ele : sol)
    {
      std::cout << ele << " ";
    }
    std::cout << "\n";
  }
  
  return 0;
}

