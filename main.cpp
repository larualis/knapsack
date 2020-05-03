#include <iostream>
#include <fstream>
#include <filesystem>
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
    minimalFunctionValues[i] = std::floor(minimalFunctionValues[i] - Problem.getSlack()[i]);
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

void checkCorrectness(Problem &problem)
{
  //! restricted DP and data extraction
  problem.reverseElements();
  
  std::vector<float> minimalFunctionValues(problem.getRestrictedFunctions().size(), 0);
  
  for (int i = 0; i < problem.getRestrictedFunctions().size(); ++i)
  {
    DP restrictedDP(problem, std::vector<int> {problem.getRestrictedFunctions()[i]});
    
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
    minimalFunctionValues[i] = std::floor(minimalFunctionValues[i] - problem.getSlack()[i]);
  }
  std::cout<<"vector for calc"<<std::endl;
  
  printVector<float>(minimalFunctionValues);
  
  //! rev DP
  revDP rDP(problem, minimalFunctionValues);
  
  auto pruningValues = rDP.run();
  
  //! final DP
  DP finalDP(problem, pruningValues);
  
  finalDP.run();
  
  auto finaleSolutionRev = finalDP.getFinalSol();
  
  //!begin normal
  problem.reverseElements();
  
  std::vector<int> functionsToCompare(problem.getNumberOfFunctions(), 0);
  
  for (int i = 0; i < problem.getNumberOfFunctions(); ++i)
  {
    functionsToCompare[i] = i + 1;
  }
  
  DP dp(problem, functionsToCompare);
  
  dp.run();
  
  std::list< std::vector<float> > finaleSolution = dp.getFinalSol();
  
  //!remove solutions that don't fit criteria
  auto sol = finaleSolution.begin();
  
  while(sol != finaleSolution.end())
  {
    bool dom = false;
    for (int i = 0; i < problem.getRestrictedFunctions().size(); ++i)
    {
      if(sol->at(problem.getRestrictedFunctions()[i]) < minimalFunctionValues[i] )
      {
        dom = true;
        break;
      }
    }
    
    if(dom)
    {
      sol = finaleSolution.erase(sol);
    }
    else
    {
      ++sol;
    }
  }
  
  //!check if equal
  bool error = false;
  
  if(finaleSolution.size() != finaleSolutionRev.size())
  {
    std::cout<<"size doesn't match"<<std::endl;
    
    std::cout<<"solutions"<<std::endl;
    
    for(auto &x: finaleSolution)
    {
      printVector<float>(x);
    }
  
    std::cout<<"solutions calc:"<<std::endl;
  
    for(auto &x: finaleSolutionRev)
    {
      printVector<float>(x);
    }
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
}

int main(int argc, char *argv[])
{
  /**
   * input of type: ../directory to files --Type= --ResFunc=
   * /directory to files contains all knapsack problem which should be solved, they should be similar
   * --Type= 0 normal 1 restricted 2 check correctness
   * --ResFunc= number of Functions which should be restricted format: 1,3,6
   **/
  int type = 0;
  
  std::vector<int> restrictedFunctions;
  
  std::string pathToFiles(argv[1]);
  
  for(int i = 1; i < argc; ++i)
  {
    std::string input (argv[i]);
    
    if (input.find("--Type=")!= std::string::npos)
    {
      type = std::stoi(input.substr(input.find('=') + 1));
    }
  
    if (input.find("--ResFunc=")!= std::string::npos)
    {
      std::string resFunc = (input.substr(input.find('=') + 1));
      //!can only handle numbers 1 to 9
      for(char const &c : resFunc)
      {
        if(c!=',')
        {
          restrictedFunctions.push_back(c - '0');
        }
      }
    }
  }
  
  for (const auto & entry : std::filesystem::directory_iterator(pathToFiles))
  {
    std::vector<float> slack {0.8};
    
    Problem problem(entry.path().string(), restrictedFunctions, slack);
  
    problem.makeMaxOrder();
  
    switch (type)
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
  }
  return 0;
}

