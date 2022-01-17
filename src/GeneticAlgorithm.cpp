//
// Created by Rafał Rodak on 14.01.2022.
//

#include "GeneticAlgorithm.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(std::vector<std::vector<int>> towns, int stop_time, int population, float crossRate,
                                   float mutationRate, TSP_Algorithm::CrossingOperations crossingOperations, TSP_Algorithm::MutationOperations mutationOperations) {
    matrix = move(towns);
    number_of_towns = matrix[0].size();
    this->stop_time = stop_time;
    this->matrix = matrix;
    this->populationSize = population;
    this->crossRate = crossRate;
    this->mutationRate = mutationRate;
    this->crossingOperations = crossingOperations;
    this->mutationOperations = mutationOperations;
    this->iteration = 0;
}


void GeneticAlgorithm::partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const {
    std::vector<int> desc1(number_of_towns, -1), desc2(number_of_towns, -1),
    map1(number_of_towns, -1), map2(number_of_towns, -1);

    int begin, end, temp;

    do {
        begin = randomIndex();
        end = randomIndex();
    } while (begin == end);

    if (begin > end)
        swap(begin, end);

    for (int i = begin; i <= end; i++) {
        desc1.at(i) = parent1.at(i);
        desc2.at(i) = parent2.at(i);
        map1[parent1.at(i)] = parent2.at(i);
        map2[parent2.at(i)] = parent1.at(i);
    }

    for (int i = 0; i < number_of_towns; i++) {
        if (desc1.at(i) == -1) {

            if (*find(desc1.begin() + begin, desc1.begin() + end, parent2.at(i)) != parent2.at(i))
                desc1.at(i) = parent2.at(i);
            else {
                temp = parent2.at(i);
                do {
                    temp = map1[temp];
                } while (*find(desc1.begin() + begin, desc1.begin() + end, temp) == temp);

                desc1.at(i) = temp;
            }
        }
        if (desc2.at(i) == -1) {
            if (*find(desc2.begin() + begin, desc2.begin() + end, parent1.at(i)) != parent1.at(i))
                desc2.at(i) = parent1.at(i);
            else {
                temp = parent1.at(i);
                do {
                    temp = map2[temp];
                } while (*find(desc2.begin() + begin, desc2.begin() + end, temp) == temp);

                desc2.at(i) = temp;
            }
        }
    }
    parent1 = desc1;
    parent2 = desc2;
}

void GeneticAlgorithm::orderedCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const {
    vector<int> temp1(number_of_towns), temp2(number_of_towns);

    int begin, end;

    temp1 = parent1;
    temp2 = parent2;

    do {
        begin = randomIndex();
        end = randomIndex();
    } while (begin == end);

    if (begin > end)
        swap(begin, end);

    int p1 = 0;
    int p2 = 0;
    for (int i = 0; i < number_of_towns; i++) {

        if (p1 == begin) p1 = end + 1;
        if (p2 == begin) p2 = end + 1;

        if (i >= begin && i <= end) {
            parent1[i] = temp2[i];
            parent2[i] = temp1[i];
        }

        if (*find(temp2.begin() + begin, temp2.begin() + end, temp1[i]) != temp1[i]) {
            parent1[p1] = temp1[i];
            p1++;
        }
        if (*find(temp1.begin() + begin, temp1.begin() + end, temp2[i]) != temp2[i]) {
            parent2[p2] = temp2[i];
            p2++;
        }
    }
}

void GeneticAlgorithm::selection(vector<PopulationElement> &population){
    int tournamentSize = 5;
    int index;
    vector<PopulationElement> nextPopulation(populationSize);
    vector<int> permutation(number_of_towns);
    int temporary_cost = INT32_MAX;
    for (int j = 0; j < populationSize; j++) {
        temporary_cost = INT32_MAX;

        // Wybór najlepszego osobnika z turnieju
        for (int k = 0; k < tournamentSize; k++) {
            index = rand() % populationSize;

            if (population.at(index).fitness < temporary_cost) {
                temporary_cost = population.at(index).fitness;
                permutation = population.at(index).route;
            }
        }
        nextPopulation.at(j).route = permutation;
        nextPopulation.at(j).fitness = temporary_cost;
    }
    // Wymiana pokoleń
    population.swap(nextPopulation);
}

void GeneticAlgorithm::apply(int chosenMutation) {
    best_cost = INT_MAX;
    vector<PopulationElement> population(populationSize);
    population = makePopulation(populationSize);
    vector<PopulationElement> next_population(population);
    clock_t start;

    string crossingOperationName, mutationOperationName, crossingRateName;
    if(crossingOperations == 1) crossingOperationName = "PMX";
    else if (crossingOperations == 2) crossingOperationName = "OX";

    if(mutationOperations == 1) mutationOperationName = "INSERT";
    else if (mutationOperations == 2) mutationOperationName = "SWAP";
    if (chosenMutation == 0) crossingRateName = "05";
    else if (chosenMutation == 1) crossingRateName = "07";
    else if (chosenMutation == 2) crossingRateName = "09";

    fstream fileCreate;
    string fileName = to_string(number_of_towns) + "_" + crossingRateName + "_" + crossingOperationName + "_" + mutationOperationName +
            + "_v"+ to_string(iteration) + ".csv";
    fileCreate.open(fileName,ios::out);
    if(!fileCreate)
    {
        cout<<"\n\nError in creating file!\n\n";
    }
    ofstream file;
    file.open(fileName);
    if(!file)
    {
        std::cout << "\n\nNie mozna zapisac, podany plik nie istnieje!\n\n";
    }
    file << "Mutacja 0." + crossingRateName << " czas "<< stop_time<< + " " + crossingOperationName + " " + mutationOperationName << endl;
    chrono::system_clock::time_point start_time = chrono::system_clock::now();
    start = std::clock();
    // Kolejne iteracje algorytmu
    while (true) {

        int64_t time_diff = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - start_time).count();
        if (time_diff >= stop_time)
            break;

        // Ocena jakości osobników
        for (auto &x : population) {
            x.fitness = getGivenRouteCost(x.route);
            if(x.fitness < best_cost) {
                best_cost = x.fitness;
                best_route = x.route;
                    file << to_string(best_cost) << " ; "<< to_string((double)(clock() - start) / (CLOCKS_PER_SEC))<< endl;


            }
        }
        // Tworzenie nowej populacji na drodze selekcji
        selection(population);

        // Krzyżowanie oraz mutowanie osobników
        for (int i = 0; i < populationSize; i += 1) {
            if ((double)rand() / RAND_MAX <= crossRate){
                int first_rand_index = rand()%(populationSize - 1) +1;
                int second_rand_index = rand()%(populationSize - 1) +1;
                while (first_rand_index == second_rand_index)
                    second_rand_index = rand()%(populationSize - 1) +1;

                switch (crossingOperations) {
                    case CrossingOperations::OX:
                        orderedCrossover(population.at(first_rand_index).route, population.at(second_rand_index).route);
                        break;
                    case CrossingOperations::PMX:
                        partiallyCrossover(population.at(first_rand_index).route, population.at(second_rand_index).route);
                        break;
                }
            }
            if((double)rand() / RAND_MAX <= mutationRate){
                mutation(population.at(i).route, mutationOperations);
            }
        }
    }

    for (auto &x : population) {
        x.fitness = getGivenRouteCost(x.route);
        if(x.fitness < best_cost) {
            best_cost = x.fitness;
            best_route = x.route;
        }
    }
    file << to_string(best_cost) << " ; "<< to_string((double)(clock() - start) / (CLOCKS_PER_SEC))<< endl;

    fileCreate.close();
    file.close();

}

void GeneticAlgorithm::mutation(std::vector<int> &route, TSP_Algorithm::MutationOperations mutationOperations) {
    int first_rand_index = randomIndex();
    int second_rand_index = randomIndex();
    while (first_rand_index == second_rand_index) // wylosowane indeksy nie nogą być identyczne
        second_rand_index = randomIndex();

    switch (mutationOperations) {
        case MutationOperations::Insert:
            if (first_rand_index < second_rand_index)
                rotate(route.begin() + first_rand_index, route.begin() + first_rand_index + 1, route.begin() + second_rand_index + 1);
            else
                rotate(route.begin() + second_rand_index, route.begin() + first_rand_index, route.begin() + first_rand_index + 1);
            break;
        case MutationOperations::Swap:
            swap(route[first_rand_index], route[second_rand_index]); // zamiana wartości pod danymi indeksami
            break;
    }
}

GeneticAlgorithm::GeneticAlgorithm(std::vector<std::vector<int>> towns, int stop_time, int population, float crossRate,
                                   float mutationRate, TSP_Algorithm::CrossingOperations crossingOperations,
                                   TSP_Algorithm::MutationOperations mutationOperations, int iteration) {

    matrix = move(towns);
    number_of_towns = matrix[0].size();
    this->stop_time = stop_time;
    this->matrix = matrix;
    this->populationSize = population;
    this->crossRate = crossRate;
    this->mutationRate = mutationRate;
    this->crossingOperations = crossingOperations;
    this->mutationOperations = mutationOperations;
    this->iteration = iteration;
}
