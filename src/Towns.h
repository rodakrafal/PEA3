//
// Created by Rafał Rodak on 15.01.2022.
//

#ifndef PEA3_TOWNS_H
#define PEA3_TOWNS_H

#include <vector>
#include <string>

class Towns
{
private:
    std::vector<std::vector<int>> towns;            // macierz z danymi
    int optimal_result;                             // optymalne rozwiązanie dla danego pliku

public:
    int getOptimalResult() const;                   // zwraca optymalne rozwiązanie
    void loadDataFromFile(const char *filename);    // wczytuje dane z pliku
    void printData();                               // wypisuje zapisane z pliku dane
    std::vector<std::vector<int>> getTowns();       // zwraca macierz -> wektor 2d z danymi
};

#endif //PEA3_TOWNS_H
