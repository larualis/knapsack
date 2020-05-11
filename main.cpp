#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>
#include "DP.h"
#include "Problem.h"
#include "revDP.h"
#include "solution/NormalSolution.h"
#include "solution/RestrictedSolution.h"
#include "solution/StatisticManager.h"

template <class x>
void printVector(std::vector<x> vec)
{
  for (auto ele : vec)
  {
    std::cout << ele << " ";
  }
  std::cout << std::endl;
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
  
  auto finaleSolutionRev = finalDP.getSolutions();
  
  //!begin normal
  problem.reverseElements();
  
  std::vector<int> functionsToCompare(problem.getNumberOfFunctions(), 0);
  
  for (int i = 0; i < problem.getNumberOfFunctions(); ++i)
  {
    functionsToCompare[i] = i + 1;
  }
  
  DP dp(problem, functionsToCompare);
  
  dp.run();
  
  std::vector< std::vector<float> > finaleSolution = dp.getSolutions();
  
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
   * --Type= 0 normal 1 restricted 2 compare 3 verifyResults
   * --ResFunc= number of Functions which should be restricted format: 1,3,6
   **/
  enum Type {normal, restricted, compare, verifyResults};
  
  Type type = Type::normal;
  
  std::vector<int> restrictedFunctions;
  
  bool printSolutions = false;
  
  std::string pathToFiles(argv[1]);
  
  for(int i = 1; i < argc; ++i)
  {
    std::string input (argv[i]);
    
    if (input.find("--Type=")!= std::string::npos)
    {
      type = static_cast<Type>(std::stoi(input.substr(input.find('=') + 1)));
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
  
    if (input.find("--PrintSolution=")!= std::string::npos)
    {
      if("true" == input.substr(input.find('=') + 1))
      {
        printSolutions = true;
      }
    }
  }
  
  StatisticManager manager(pathToFiles);
  
  StatisticManager secondManager(pathToFiles);
  
  int numberOfKnapsacksToSolve = 0;
  
  for (const auto & entry : std::filesystem::directory_iterator(pathToFiles))
  {
    if(entry.path().string().find("knapsack") != std::string::npos)
    {
      ++numberOfKnapsacksToSolve;
    }
  }
  
  for (int numberKnapsack = 1; numberKnapsack <= numberOfKnapsacksToSolve; ++numberKnapsack)
  {
    std::cout<<"solve Knapsack: "<<numberKnapsack<<"\r"<<"\n"<<std::flush;
    
    std::vector<float> slack {0.8};
    
    Problem problem(pathToFiles + "/knapsack" + std::to_string(numberKnapsack) + ".txt", restrictedFunctions, slack);
  
    problem.makeMaxOrder();
    
    switch (type)
    {
      case normal:
      {
        NormalSolution sol(problem);
        
        sol.generateSolutin();
        
        manager.addSolution(sol);
        break;
      }
      case restricted:
      {
        problem.reverseElements();
  
        RestrictedSolution sol(problem);
        
        sol.generateSolutin();
  
        manager.addSolution(sol);
        break;
      }
      case compare:
      {
        if(!std::filesystem::exists(pathToFiles + "/results_normal.txt"))
        {
          NormalSolution normalSol(problem);
  
          normalSol.generateSolutin();
  
          manager.addSolution(normalSol);
        }
        
        problem.reverseElements();
  
        RestrictedSolution restSol(problem);
  
        restSol.generateSolutin();
        
        secondManager.addSolution(restSol);
        break;
      }
      case verifyResults:
      {
        checkCorrectness(problem);
        break;
      }
      default:
        std::cout<<"wrong argument"<<std::endl;
        return 0;
    }
  }
  
  if(type != Type::compare)
  {
    manager.printStatistics(printSolutions);
  }
  
  if(type == Type::normal)
  {
    manager.writeStatistics();
  }
  
  if(type == Type::compare)
  {
    if(std::filesystem::exists(pathToFiles + "/results_normal.txt"))
    {
      manager.readFromFile(pathToFiles + "/results_normal.txt");
    }
    else
    {
      manager.writeStatistics();
    }
    
    manager.printStatistics(printSolutions);
    
    secondManager.printStatistics(printSolutions);
    
    secondManager.printCompareToOtherSolutions(manager, false);
  }

  return 0;
}

