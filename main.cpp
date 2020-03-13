#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DP.h"
#include "problem.h"


int main()
{
  std::vector<std::pair<int, int>> restrictedFunctions {{1,10}, {2,10}};
  
  std::string data = "../python/knapsack.txt";
  
  problem Problem(data, 150, 2, restrictedFunctions);
  
  DP restrictedDP(Problem);
  
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

