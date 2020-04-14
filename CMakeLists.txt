cmake_minimum_required(VERSION 3.10)
project(Knapsack)

set(CMAKE_CXX_STANDARD 14)

add_executable(Knapsack main.cpp DP.cpp DP.h revDP.cpp revDP.h Problem.cpp Problem.h ElementManager.cpp ElementManager.h)