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

/**
 * Metoda wykonująca krzyżowanie PMX dla zadanych dwóch rodzicach
 * @param parent1 pierwszy rodzic, będący pewną losowo wybraną permutacją ścieżki
 * @param parent2 drugi rodzic, będący pewną losowo wybraną permutacją ścieżki
 */
void GeneticAlgorithm::partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const {
    // tworzenie tymczasowych wektorów przetrzymującym tworzących potomków oraz tablice mapujące
    std::vector<int> desc1(number_of_towns, -1), desc2(number_of_towns, -1),
    map1(number_of_towns, -1), map2(number_of_towns, -1);
    // zmienne do utworzenia sekcji dopasowanej
    int begin, end, temp;
    // losowanie sekcji
    do {
        begin = randomIndex();
        end = randomIndex();
    } while (begin == end);

    if (begin > end)
        swap(begin, end);
    // kopiowanie sekcji dopasowanej
    for (int i = begin; i <= end; i++) {
        desc1.at(i) = parent1.at(i);
        desc2.at(i) = parent2.at(i);
        map1[parent1.at(i)] = parent2.at(i);
        map2[parent2.at(i)] = parent1.at(i);
    }
    // kopiowanie pozostałych elementów
    for (int i = 0; i < number_of_towns; i++) {
        // jeżeli takowy element jeszcze nie znajduje się w potomku pierwszym
        if (desc1.at(i) == -1) {
            // jeżeli nie występuje konflikt w sekcji dopasowanej to kopiuj
            if (*find(desc1.begin() + begin, desc1.begin() + end, parent2.at(i)) != parent2.at(i))
                desc1.at(i) = parent2.at(i);
            // jeżeli występuje konflikt w sekcji dopasowanej to kopiuj z uwzględnieniem mapowania w tablicy
            else {
                temp = parent2.at(i);
                // sprawdzaj do momentu aż nie będzie konfliktów
                do {
                    temp = map1[temp];
                } while (*find(desc1.begin() + begin, desc1.begin() + end, temp) == temp);

                desc1.at(i) = temp;
            }
        }

        // jeżeli takowy element jeszcze nie znajduje się w potomku drugim
        if (desc2.at(i) == -1) {
            // jeżeli nie występuje konflikt w sekcji dopasowanej to kopiuj
            if (*find(desc2.begin() + begin, desc2.begin() + end, parent1.at(i)) != parent1.at(i))
                desc2.at(i) = parent1.at(i);
            // jeżeli występuje konflikt w sekcji dopasowanej to kopiuj z uwzględnieniem mapowania w tablicy
            else {
                temp = parent1.at(i);
                // sprawdzaj do momentu aż nie będzie konfliktów
                do {
                    temp = map2[temp];
                } while (*find(desc2.begin() + begin, desc2.begin() + end, temp) == temp);

                desc2.at(i) = temp;
            }
        }
    }
    // podmień rodziców z potomkami
    parent1 = desc1;
    parent2 = desc2;
}
/**
 * Metoda wykonująca krzyżowanie OX dla zadanych dwóch rodzicach
 * @param parent1 pierwszy rodzic, będący pewną losowo wybraną permutacją ścieżki
 * @param parent2 drugi rodzic, będący pewną losowo wybraną permutacją ścieżki
 */
void GeneticAlgorithm::orderCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const {
    // tworzenie tymczasowych wektorów przetrzymującym tworzących potomków
    vector<int> temp1(number_of_towns), temp2(number_of_towns);
    // zmienne do utworzenia sekcji dopasowanej
    int begin, end;

    temp1 = parent1;
    temp2 = parent2;
    // losowanie sekcji
    do {
        begin = randomIndex();
        end = randomIndex();
    } while (begin == end);

    if (begin > end)
        swap(begin, end);

    int p1 = 0;
    int p2 = 0;
    for (int i = 0; i < number_of_towns; i++) {
        // sprawdzamy, czy wchodzimy w sekcje dopasowaną, jeżeli tak to ustawiamy zmienne na następny element po sekcji
        if (p1 == begin) p1 = end + 1;
        if (p2 == begin) p2 = end + 1;

        // kopiowanie sekcji dopasowanej
        if (i >= begin && i <= end) {
            parent1[i] = temp2[i];
            parent2[i] = temp1[i];
        }
        // kopiowanie elementu, jeżeli nie jest w sekcji dopasowanej
        if (*find(temp2.begin() + begin, temp2.begin() + end, temp1[i]) != temp1[i]) {
            parent1[p1] = temp1[i];
            p1++;
        }
        // kopiowanie elementu, jeżeli nie jest w sekcji dopasowanej
        if (*find(temp1.begin() + begin, temp1.begin() + end, temp2[i]) != temp2[i]) {
            parent2[p2] = temp2[i];
            p2++;
        }
    }
}
/**
 * Metoda wykonująca sekcję turniejową na podstawie zadanej populacji
 * @param population aktualna populacja na której wykonywany jest algorytm, jest to wektor struktury PopulationElement
 */
void GeneticAlgorithm::selection(vector<PopulationElement> &population){
    // ustawiamy rozmiar tournament na 5
    int tournamentSize = 5;
    int index;
    // tymczasowa zmienna zawierająca następna populacje
    vector<PopulationElement> nextPopulation(populationSize);
    // tworzenie zmiennej tymczasowej zawierającej ścieżkę
    vector<int> temporary_route(number_of_towns);
    int temporary_cost = INT32_MAX;
    // wykonujemy dla całej populacji
    for (int j = 0; j < populationSize; j++) {
        temporary_cost = INT32_MAX;

        // Wybór najlepszego osobnika z turnieju
        for (int k = 0; k < tournamentSize; k++) {
            index = rand() % populationSize;
            // jeżeli nowo wylosowany zawodnik jest lepszy od poprzedniego
            if (population.at(index).fitness < temporary_cost) {
                temporary_cost = population.at(index).fitness;
                temporary_route = population.at(index).route;
            }
        }
        // zapisujemy wartości w tymczasowej zmiennej zawierającej następna populacje
        nextPopulation.at(j).route = temporary_route;
        nextPopulation.at(j).fitness = temporary_cost;
    }
    // zmiana pokoleń
    population.swap(nextPopulation);
}

/**
 * Główna metoda odpowiadają za logikę algorytmu, zawiera również pętlę będące "sercem" programu
 */
void GeneticAlgorithm::execute() {
    // przypisujemy początkowy najmniejszy koszt na nieskończoność
    best_cost = INT_MAX;
    // wektor PopulationElement reprezentujący populację
    vector<PopulationElement> population(populationSize);
    // tworzenie losowej populacji metodą makePopulation
    population = makePopulation(populationSize);
    vector<PopulationElement> next_population(population);
    clock_t start;
    // pomocnicze zmienne do odpowiedniego zapisywania plików z wynikami
    string crossingOperationName, mutationOperationName, crossingRateName;
    fstream fileCreate;
    ofstream file;

    if(iteration!=0){
        if(crossingOperations == 1) crossingOperationName = "PMX";
        else if (crossingOperations == 2) crossingOperationName = "OX";

        if(mutationOperations == 1) mutationOperationName = "INSERT";
        else if (mutationOperations == 2) mutationOperationName = "SWAP";
        // tworzenie oraz otwieranie pliku z wynikami
        string fileName = to_string(number_of_towns) + "_" + crossingRateName + "_" + crossingOperationName + "_" + mutationOperationName +
                          + "_v"+ to_string(iteration) + ".csv";
        fileCreate.open(fileName,ios::out);
        if(!fileCreate)
        {
            cout<<"\n\nError in creating file!\n\n";
        }
        file.open(fileName);
        if(!file)
        {
            std::cout << "\n\nNie mozna zapisac, podany plik nie istnieje!\n\n";
        }
        file << "Mutacja 0." + crossingRateName << " czas "<< stop_time<< + " " + crossingOperationName + " " + mutationOperationName << endl;
    }


    // rozpoczęcie pomiaru czas
    chrono::system_clock::time_point start_time = chrono::system_clock::now();
    start = std::clock();
    // kolejne iteracje algorytmu
    while (true) {
        // warunek stopu
        int64_t time_diff = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - start_time).count();
        if (time_diff >= stop_time)
            break;

        // ocena jakości osobników
        for (auto &x : population) {
            // obliczamy fitness będący kosztem przejścia ścieżki
            x.fitness = getGivenRouteCost(x.route);
            // zapamiętywanie najlepszego wyniku
            if(x.fitness < best_cost) {
                best_cost = x.fitness;
                best_route = x.route;
                if(iteration!=0)file << to_string(best_cost) << " ; "<< to_string((double)(clock() - start) / (CLOCKS_PER_SEC))<< endl;

            }
        }
        // tworzenie nowej populacji na drodze selekcji turniejowej
        selection(population);

        // krzyżowanie oraz mutowanie osobników
        for (int i = 0; i < populationSize; i += 1) {
            // jeżeli na drodze losowej zostanie spełniony warunek
            if ((double)rand() / RAND_MAX <= crossRate){
                // losowanie indeksu elementu z populacji
                int first_rand_index = rand()%(populationSize - 1) +1;
                int second_rand_index = rand()%(populationSize - 1) +1;
                while (first_rand_index == second_rand_index)
                    second_rand_index = rand()%(populationSize - 1) +1;

                switch (crossingOperations) {
                    case CrossingOperations::OX:
                        orderCrossover(population.at(first_rand_index).route, population.at(second_rand_index).route);
                        break;
                    case CrossingOperations::PMX:
                        partiallyCrossover(population.at(first_rand_index).route, population.at(second_rand_index).route);
                        break;
                }
            }
            // jeżeli na drodze losowej zostanie spełniony warunek
            if((double)rand() / RAND_MAX <= mutationRate){
                // wykonanie metody odpowiedzialnej za mutowanie populacji wybraną operacją mutacji
                mutation(population.at(i).route, mutationOperations);
            }
        }
    }
    // na koniec sprawdzanie, czy wynik się nie zmienił
    for (auto &x : population) {
        x.fitness = getGivenRouteCost(x.route);
        if(x.fitness < best_cost) {
            best_cost = x.fitness;
            best_route = x.route;
        }
    }
    if(iteration!=0) {
        file << to_string(best_cost) << " ; "<< to_string((double)(clock() - start) / (CLOCKS_PER_SEC))<< endl;
        fileCreate.close();
        file.close();
    }

}
/**
 * Metoda odpowiedzialna za mutowanie podanej ścieżki metodą insert badź swap
 * @param route losowo wybrana ścieżka na której wykonana ma zostać mutacja
 * @param mutationOperations wybrana operacja mutacji insert, bądź swap
 */
void GeneticAlgorithm::mutation(std::vector<int> &route, TSP_Algorithm::MutationOperations mutationOperations) {
    int first_rand_index = randomIndex();
    int second_rand_index = randomIndex();
    while (first_rand_index == second_rand_index) // wylosowane indeksy nie nogą być identyczne
        second_rand_index = randomIndex();

    switch (mutationOperations) {
        case MutationOperations::Insert:
            if (first_rand_index < second_rand_index) // przeniesienie wartości wskazywanej przez pierwszy indeks w miejsce wskazywane przez drugi indeks
                rotate(route.begin() + first_rand_index, route.begin() + first_rand_index + 1, route.begin() + second_rand_index + 1);
            else
                rotate(route.begin() + second_rand_index, route.begin() + first_rand_index, route.begin() + first_rand_index + 1);
            break;
        case MutationOperations::Swap:
            swap(route[first_rand_index], route[second_rand_index]); // zamiana wartości pod danymi indeksami
            break;
    }
}


