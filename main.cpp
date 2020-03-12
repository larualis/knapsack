#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DP.h"

void readData(int numberOfFunctions, std::string data, std::vector<std::vector<float>>& elements)
{
  std::ifstream file;
  
  file.open(data);
  
  std::vector<float> element(numberOfFunctions + 1, 0);
  
  while (!file.eof())
  {
    for (int i = 0; i <= numberOfFunctions; ++i)
    {
      file >> element[i];
    }
    elements.push_back(element);
  }
  file.close();
}

int main()
{
  int numberOfFunctions = 2;
  
  float capacity = 150;
  
  std::vector<std::pair<int, int>> restrictedFunctions {{1,10}, {2,10}};
  
  std::string data = "../python/knapsack.txt";
  
  std::vector<std::vector<float>> elements;
  
  readData(numberOfFunctions, data, elements);
  
  DP problem(elements, capacity, numberOfFunctions);
  
  problem.run();
  
  auto solutions = problem.getSolutions();
  
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

