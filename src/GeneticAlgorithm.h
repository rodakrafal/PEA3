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

    int populationSize;     // rozmiar populacja
    float crossRate;        // współczynnik krzyżowania
    float mutationRate;     // współczynnik mutacji
    int iteration;          // pomocnicza zmienna do zapisywania wyników

public:

    GeneticAlgorithm(std::vector<std::vector<int>> towns, int stop_time, int population, float crossRate, float mutationRate, CrossingOperations crossingOperations, MutationOperations mutationOperations, int iteration);
    void orderCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;        // krzyżowanie OX
    void partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;    // krzyżowanie PMX
    void execute();                                                       // metoda wykonująca algorytm
    void selection(std::vector<PopulationElement> &population);                             //selekcja turniejowa
    void mutation(std::vector<int> &route, MutationOperations mutationOperations);          //metoda wykonującą odpowiednio wybraną metodę mutacji insert bądź swap
};


#endif //PEA3_GENETICALGORITM_H
