cmake_minimum_required(VERSION 3.10)
project(Knapsack)

set(CMAKE_CXX_STANDARD 17)

link_libraries(stdc++fs)

add_executable(Knapsack
        main.cpp
        revDP.cpp
        revDP.h
        Problem.cpp
        Problem.h
        ElementManager.cpp
        ElementManager.h
        solution/Solution.cpp
        solution/Solution.h
        solution/StatisticManager.cpp
        solution/StatisticManager.h
        pruning_rules/ParetoPruning.cpp
        pruning_rules/ParetoPruning.h
        pruning_rules/RestrictedPruning.cpp
        pruning_rules/RestrictedPruning.h
        pruning_rules/UpperBoundPruning.cpp
        pruning_rules/UpperBoundPruning.h
        pruning_rules/RemainingWeightPruning.cpp
        pruning_rules/RemainingWeightPruning.h
        pruning_rules/Pruning.cpp
        pruning_rules/Pruning.h
        SolutionComparer.h
        DP/NormalDP.cpp
        DP/NormalDP.h
        DP/RestrictedDP.cpp
        DP/RestrictedDP.h
        pruning_rules/ApproxPruning.cpp
        pruning_rules/ApproxPruning.h
        DP/ApproxDP.cpp
        DP/ApproxDP.h )