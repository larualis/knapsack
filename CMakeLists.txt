cmake_minimum_required(VERSION 3.15)
project(Knapsack)

set(CMAKE_CXX_STANDARD 14)

add_executable(Knapsack main.cpp DP.cpp DP.h revDP.cpp revDP.h)