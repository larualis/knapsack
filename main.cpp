#include <iostream>
#include <filesystem>
#include <vector>
#include "Problem.h"
#include "solution/StatisticManager.h"

int numberOfFunctionsInInput(std::string& filePath)
{
  std::ifstream file;
  
  file.open(filePath);
  
  std::string line;
  
  int numberOfFunctionsInInput = -1;
  
  std::getline(file, line);
  
  std::stringstream ss(line);
  
  float val;
  
  while(ss >> val)
  {
    ++numberOfFunctionsInInput;
  }
  file.close();
  
  return numberOfFunctionsInInput;
}

int main(int argc, char *argv[])
{
  /**
   * input of type: ../directory_to_files --Type= --ResFunc= --Slack= --Error= --PrintSolution=
   * /directory_to_files contains all knapsack problems which should be solved, they should be similar
   * --Type= 0 normal 1 restricted 2 approx 3 approx & restricted 4 compare normal restricted
   * --ResFunc= number of functions which should be restricted, format: 1,3,6
   * --Slack= percentage of best single value, used for all ResFunc
   * --Error= needed for 1+Error approximation
   * --PrintSolution= prints the values of all knapsack solutions
   **/
  enum Type {normal, restricted, approx, approxRestricted, compare, verifyResults};
  
  Type type = Type::normal;
  
  std::vector<int> restrictedFunctions;
  
  bool printSolutions = false;
  
  std::vector<float> slack;
  
  float error = 0;
  
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
  
    if (input.find("--Slack=")!= std::string::npos)
    {
      slack.push_back(std::stof(input.substr(input.find('=') + 1)));
    }
  
    if (input.find("--Error=")!= std::string::npos)
    {
      error = std::stof(input.substr(input.find('=') + 1));
    }
  }
  
  StatisticManager manager(pathToFiles);
  
  StatisticManager secondManager(pathToFiles);
  
  int numberOfKnapsacksToSolve = 0;
  
  for (const auto & entry : std::filesystem::directory_iterator(pathToFiles))
  {
    int posOfFileName = entry.path().string().rfind("/") + 1;
    
    if(entry.path().string().substr(posOfFileName).find("knapsack") != std::string::npos)
    {
      ++numberOfKnapsacksToSolve;
    }
  }
  
  for (int numberKnapsack = 1; numberKnapsack <= numberOfKnapsacksToSolve; ++numberKnapsack)
  {
    std::cout<<"solve Knapsack: "<<numberKnapsack<<"\r"<<"\n"<<std::flush;
    
    std::string filePath = pathToFiles + "/knapsack" + std::to_string(numberKnapsack) + ".txt";
    
    if(numberOfFunctionsInInput(filePath) > NUMBEROFFUNCTIONS)
    {
      std::cout<<"macro number for Element size is too small" << std::endl;
      
      std::cout<< "macro number = " << NUMBEROFFUNCTIONS << std::endl;
      
      std::cout << "input number = "<< numberOfFunctionsInInput(filePath) << std::endl;
  
      return 0;
    }
    
    Problem problem(filePath, restrictedFunctions, slack, error);
    
    problem.makeMaxOrder();
  
    Solution sol(problem);
  
    switch (type)
    {
      case normal:
      {
        sol.makeNormalSolution();
        
        manager.addSolution(sol);
        break;
      }
      case restricted:
      {
        sol.makeRestrictedSolution();
  
        manager.addSolution(sol);
        break;
      }
      case approx:
      {
        sol.makeApproxSolution();
  
        manager.addSolution(sol);
        break;
      }
      case approxRestricted:
      {
        sol.makeApproxRestrictedSolution();
    
        manager.addSolution(sol);
        break;
      }
      case compare:
      {
        if(!std::filesystem::exists(pathToFiles + "/results_normal.txt"))
        {
          Solution normalSol(problem);
  
          normalSol.makeNormalSolution();
  
          secondManager.addSolution(normalSol);
        }
        
        sol.makeRestrictedSolution();
  
        manager.addSolution(sol);
        break;
      }
      default:
        std::cout<<"wrong argument"<<std::endl;
        return 0;
    }
  }
  
  std::cout<<std::endl<<"Results:"<<std::endl;
  
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
      secondManager.readFromFile(pathToFiles + "/results_normal.txt");
    }
    else
    {
      secondManager.writeStatistics();
    }
  
    secondManager.printStatistics(printSolutions);
  
    manager.printStatistics(printSolutions);
    
    manager.printCompareToOtherSolutions(secondManager, false);
  }

  return 0;
}

