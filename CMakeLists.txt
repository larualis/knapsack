cmake_minimum_required(VERSION 3.10)
project(Knapsack)

set(CMAKE_CXX_STANDARD 17)

link_libraries(stdc++fs)

add_executable(Knapsack main.cpp DP.cpp DP.h revDP.cpp revDP.h Problem.cpp Problem.h ElementManager.cpp ElementManager.h solution/Solution.h solution/NormalSolution.cpp solution/NormalSolution.h solution/RestrictedSolution.cpp solution/RestrictedSolution.h solution/StatisticManager.cpp solution/StatisticManager.h)