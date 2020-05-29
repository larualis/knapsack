//
// Created by rick on 27.05.20.
//

#ifndef KNAPSACK_SOLUTIONCOMPARER_H
#define KNAPSACK_SOLUTIONCOMPARER_H


#include "ElementManager.h"

namespace solutionComparer
{
  inline bool dominates(static_vector &sol1, static_vector &sol2)
  {
    int numberOfFunctions = sol1.size() - 1;
    for (int i = 1; i <= numberOfFunctions; ++i)
    {
      if(sol1[i] < sol2[i])
      {
        return false;
      }
    }
    
    return true;
  }
  
  inline bool dlex(static_vector &sol1, static_vector &sol2)
  {
    int numberOfFunctions = sol1.size() - 1;
    
    for (int i = 1; i <= numberOfFunctions; ++i)
    {
      if (sol1[i] != sol2[i])
      {
        return sol1[i] > sol2[i];
      }
    }
    return true;
  }
  
  
  inline bool lex(static_vector &sol1, static_vector &sol2)
  {
    if (sol1[0] < sol2[0])
    {
      return true;
    }
    
    return sol1[0] == sol2[0] and dlex(sol1, sol2);
  }
}

#endif //KNAPSACK_SOLUTIONCOMPARER_H
