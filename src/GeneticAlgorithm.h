//
// Created by Rafał Rodak on 14.01.2022.
//

#ifndef PEA3_GENETICALGORITM_H
#define PEA3_GENETICALGORITM_H

#include "TSP_Algorithm.h"
#include <vector>


class GeneticAlgorithm : public TSP_Algorithm
{
private:

    int populationSize;
    float crossRate;
    float mutationRate;

public:

    GeneticAlgorithm(std::vector<std::vector<int>> towns, int stop_time, int population, float crossRate, float mutationRate, CrossingOperations crossingOperations, MutationOperations mutationOperations);
    void orderedCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;
    void partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;
    void apply();
    void selection(std::vector<PopulationElement> &population);
    void mutation(std::vector<int> &route, MutationOperations mutationOperations);
};


#endif //PEA3_GENETICALGORITM_H
