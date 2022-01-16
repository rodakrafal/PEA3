//
// Created by Rafał Rodak on 15.01.2022.
//

#include "TSP_Algorithm.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

void TSP_Algorithm::printBestRoute() {
    for (auto i = 0; i < best_route.size() - 1; i++)
        cout << best_route[i] << " -> ";
    cout << best_route[best_route.size() - 1] << " -> ";
    cout << "0" <<endl;
    cout << "Cost = " << best_cost << endl;
}

int TSP_Algorithm::getBestRouteCost() const {
    return best_cost;
}

int TSP_Algorithm::getGivenRouteCost(std::vector<int> givenRoute) const {
    int path_distance = 0;
    // zliczenie sumy odległości w ścieżce aż do danego wierzchołka
    for (int i = 0; i < number_of_towns - 1; i++)
        path_distance += matrix[givenRoute[i]][givenRoute[i + 1]];
    path_distance += matrix[givenRoute[number_of_towns - 1]][givenRoute[0]];
    return path_distance;
}

int TSP_Algorithm::randomIndex() const {
    return rand() % (number_of_towns - 1) + 1;
}

vector<PopulationElement> TSP_Algorithm::makePopulation(int sizeOfPopulation) {
    vector<int> permutation(number_of_towns);
    vector<PopulationElement> population(sizeOfPopulation);

    for (int i = 0; i < number_of_towns; i++) permutation.at(i) = i;

    for (int i = 0; i < sizeOfPopulation; i++) {
        shuffle(permutation.begin() +1, permutation.end(),mt19937(random_device()()));
        population[i].route = permutation;
    }
    return population;
}
