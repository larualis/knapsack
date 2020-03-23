#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DP.h"
#include "problem.h"
#include "revDP.h"


int main()
{
  std::vector<int> restrictedFunctions {1,2};
  
  std::vector<float> slack {200, 200};
  
  std::string data = "../python/knapsack.txt";
  
  problem Problem(data, 250, 4, restrictedFunctions, slack);
  
  DP restrictedDP(Problem, restrictedFunctions);
  
  restrictedDP.run();
  
  auto solutions = restrictedDP.getSolutions();
  
  std::vector<float> test = solutions[0];
  
  for (auto sol: solutions)
  {
    for(int i = 1; i <= Problem.getRestrictedFunctions().size(); ++i)
    {
      if(test[i] > sol[i])
      {
        test[i] = sol[i];
      }
    }
  }
  
  for (int i = 1; i < test.size(); ++i )
  {
    test[i] = test[i] - slack[i - 1];
  }
  
  revDP rDP(Problem, test);
  
  rDP.run();
  
  DP finalDP(Problem, rDP.getPruningValues());
  
  finalDP.run();
  
  auto finaleSolution = finalDP.getSolutions();
  
  std::sort(finaleSolution.begin(), finaleSolution.end(), [](const std::vector<float> & a, const std::vector<float> & b){ return a[1] < b[1]; });
  
  for (auto sol: finaleSolution)
  {
    for (auto ele : sol)
    {
      std::cout << ele << " ";
    }
    std::cout << "\n";
  }
  
  return 0;
}

