//
// Created by rick on 27.05.20.
//

#include "SolutionComparer.h"

SolutionComparer::SolutionComparer(int numberOfFunctions):
numberOfFunctions_(numberOfFunctions)
{}

bool SolutionComparer::dominates(static_vector &sol1, static_vector &sol2)
{
  for (int i = 1; i <= numberOfFunctions_; ++i)
  {
    if(sol1[i] < sol2[i])
    {
      return false;
    }
  }
  
  return true;
}

bool SolutionComparer::lex(static_vector &sol1, static_vector &sol2)
{
  if (sol1[0] < sol2[0])
  {
    return true;
  }
  
  return sol1[0] == sol2[0] and dlex(sol1, sol2);
}

bool SolutionComparer::dlex(static_vector &sol1, static_vector &sol2)
{
  for (int i = 1; i <= numberOfFunctions_; ++i)
  {
    if (sol1[i] != sol2[i])
    {
      return sol1[i] > sol2[i];
    }
  }
  return true;
}