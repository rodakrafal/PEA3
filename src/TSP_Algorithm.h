//
// Created by Rafał Rodak on 15.01.2022.
//

#ifndef PEA3_TSP_ALGORITHM_H
#define PEA3_TSP_ALGORITHM_H

#include <vector>

struct PopulationElement
{
    std::vector<int> route;
    int fitness = INT_MAX;
};

class TSP_Algorithm {
public:

    enum CrossingOperations{                // enum z dostępnymi operacjami krzyżowania
        PMX = 1,
        OX = 2,
    };

    enum MutationOperations{                // enum z dostępnymi operacjami mutacji
        Insert = 1,
        Swap = 2,
    };

    std::vector<std::vector<int>> matrix; // macierz
    int number_of_towns;    // ilość wierzchołków
    std::vector<int> best_route; // najlepsza ścieżka
    int best_cost;           // koszt najlepszej ścieżki
    int stop_time;          // czas w sekundach po jakim algorytm się zatrzyma
    CrossingOperations crossingOperations;
    MutationOperations mutationOperations;
public:

    void printBestRoute();                                           // wyświetla najlepszą ścieżkę
    int getBestRouteCost() const;                                    // wyświetla najlepszy koszt ścieżki
    int getGivenRouteCost(std::vector<int> givenRoute) const;        // zwraca koszt najlepszej ścieżki
    std::vector<PopulationElement> makePopulation(int sizeOfPopulation);
    int randomIndex() const;

};


#endif //PEA3_TSP_ALGORITHM_H
