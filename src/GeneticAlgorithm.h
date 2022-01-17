//
// Created by Rafał Rodak on 14.01.2022.
//

#ifndef PEA3_GENETICALGORITM_H
#define PEA3_GENETICALGORITM_H

#include "TSP_Algorithm.h"
#include <vector>
#include <string>


class GeneticAlgorithm : public TSP_Algorithm
{
private:

    int populationSize;
    float crossRate;
    float mutationRate;
    int iteration;

public:

    GeneticAlgorithm(std::vector<std::vector<int>> towns, int stop_time, int population, float crossRate, float mutationRate, CrossingOperations crossingOperations, MutationOperations mutationOperations);
    GeneticAlgorithm(std::vector<std::vector<int>> towns, int stop_time, int population, float crossRate, float mutationRate, CrossingOperations crossingOperations, MutationOperations mutationOperations, int iteration);
    void orderedCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;
    void partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;
    void apply(int chosenMutation);
    void selection(std::vector<PopulationElement> &population);
    void mutation(std::vector<int> &route, MutationOperations mutationOperations);
};


#endif //PEA3_GENETICALGORITM_H
