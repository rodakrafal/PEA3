cmake_minimum_required(VERSION 3.20)
project(PEA3)

set(CMAKE_CXX_STANDARD 20)

add_executable(PEA3 src/main.cpp src/GeneticAlgorithm.cpp src/GeneticAlgorithm.h src/Towns.cpp src/Towns.h src/TSP_Algorithm.cpp src/TSP_Algorithm.h)

file(COPY test_files/ftv55.txt test_files/ftv47.tsp test_files/ftv170.tsp test_files/rbg403.tsp
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})