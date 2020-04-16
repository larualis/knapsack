#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include "DP.h"
#include "Problem.h"
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

void advancedPruning(Problem &Problem)
{
  //! restricted DP and data extraction
  auto t1 = std::chrono::high_resolution_clock::now();
  
  std::vector<float> minimalFunctionValues(Problem.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < Problem.getRestrictedFunctions().size(); ++i)
  {
    DP restrictedDP(Problem, std::vector<int> {Problem.getRestrictedFunctions()[i]});
  
    restrictedDP.run();
  
    auto solutions = restrictedDP.getFinalSol();
  
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
    minimalFunctionValues[i] = std::floor(minimalFunctionValues[i] * Problem.getSlack()[i]);
  }
  std::cout<<"vector for calc"<<std::endl;
  
  printVector<float>(minimalFunctionValues);
  
  //! rev DP
  revDP rDP(Problem, minimalFunctionValues);
  
  auto pruningValues = rDP.run();
  
  //! final DP
  DP finalDP(Problem, pruningValues);
  
  finalDP.run();
  
  auto t2 = std::chrono::high_resolution_clock::now();
  
  auto finaleSolution = finalDP.getFinalSol();
  
  for (auto sol: finaleSolution)
  {
    printVector<float>(sol);
  }
  
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  
  std::cout <<"Took: " << duration <<"\n";
  
  std::cout <<"found Solutions: " << finaleSolution.size() <<"\n";
}

void standartDP(Problem &Problem, std::vector<int> functionsToCompare)
{
  DP dp(Problem, functionsToCompare);
  
  auto t1 = std::chrono::high_resolution_clock::now();
  dp.run();
  auto t2 = std::chrono::high_resolution_clock::now();
  
  
  auto finaleSolution = dp.getFinalSol();
  
  for (auto &sol: finaleSolution)
  {
    printVector<float>(sol);
  }
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  
  std::cout <<"Took: " << duration <<"\n";
  
  std::cout <<"found Solutions: " << finaleSolution.size() <<"\n";
}

void checkCorrectness(Problem &Problem)
{
  //! restricted DP and data extraction
  std::vector<float> minimalFunctionValues(Problem.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < Problem.getRestrictedFunctions().size(); ++i)
  {
    DP restrictedDP(Problem, std::vector<int> {Problem.getRestrictedFunctions()[i]});
    
    restrictedDP.run();
    
    auto solutions = restrictedDP.getFinalSol();
    
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
    minimalFunctionValues[i] = std::floor(minimalFunctionValues[i] * Problem.getSlack()[i]);
  }
  std::cout<<"vector for calc"<<std::endl;
  
  printVector<float>(minimalFunctionValues);
  
  //! rev DP
  revDP rDP(Problem, minimalFunctionValues);
  
  auto pruningValues = rDP.run();
  
  //! final DP
  DP finalDP(Problem, pruningValues);
  
  finalDP.run();
  
  auto finaleSolutionRev = finalDP.getFinalSol();
  
  //!begin normal
  
  std::vector<int> functionsToCompare(Problem.getNumberOfFunctions(), 0);
  
  for (int i = 0; i < Problem.getNumberOfFunctions(); ++i)
  {
    functionsToCompare[i] = i + 1;
  }
  
  DP dp(Problem, functionsToCompare);
  
  dp.run();
  
  std::list< std::vector<float> > finaleSolution = dp.getFinalSol();
  
  //!check if equal
  for (auto sol = finaleSolution.begin(); sol != finaleSolution.end(); ++sol)
  {
    for (int i = 0; i < Problem.getRestrictedFunctions().size(); ++i)
    {
      if(sol->at(Problem.getRestrictedFunctions()[i]) < minimalFunctionValues[i] )
      {
        sol = finaleSolution.erase(sol);
        
        --sol;
      }
    }
  }
  
  bool error = false;
  
  if(finaleSolution.size() != finaleSolutionRev.size())
  {
    std::cout<<"size doesn't match"<<std::endl;
    error = true;
  }
  else
  {
    auto iterator1 = finaleSolution.begin();
    auto iterator2 = finaleSolutionRev.begin();
    
    
    while(iterator1 != finaleSolution.end())
    {
      for (int i = 0; i < iterator1->size(); ++i)
      {
        if(iterator1->at(i) != iterator2->at(i))
        {
          std::cout<<"entrys don't mach"<<std::endl;
          error = true;
          break;
        }
      }
      
      ++iterator1;
      ++iterator2;
    }
  
    if(error)
    {
      printVector<float>(*iterator1);
      printVector<float>(*iterator2);
    }
    else
    {
      std::cout<<"Solutions match"<<std::endl;
    }
  }
  
  while(!pruningValues.empty())
  {
    pruningValues.erase(pruningValues.end() - 1);
  }
};

int main(int argc, char **argv)
{
  //!input
  std::vector<int> restrictedFunctions {1,2};
  
  std::vector<float> slack {0.95,0.8};
  
  std::string data = "../python/knapsack.txt";
  
  Problem problem(data, 3, restrictedFunctions, slack);
  
  problem.makeMaxOrder();
  
  switch (std::stoi(argv[1]))
  {
    case 0:
      {
        std::vector<int> functionsToCompare(problem.getNumberOfFunctions(), 0);
  
        for (int i = 0; i < problem.getNumberOfFunctions(); ++i)
        {
          functionsToCompare[i] = i + 1;
        }
  
        standartDP(problem, functionsToCompare);
        break;
      }
    case 1:
      {
        problem.reverseElements();
        
        advancedPruning(problem);
        break;
      }
    case 2:
    {
      checkCorrectness(problem);
      break;
    }
    default:
      std::cout<<"wrong argument"<<std::endl;
  }
  return 0;
}

