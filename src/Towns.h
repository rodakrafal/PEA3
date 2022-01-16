//
// Created by Rafał Rodak on 15.01.2022.
//

#ifndef PEA3_TOWNS_H
#define PEA3_TOWNS_H

#include <vector>

class Towns
{
private:
    std::vector<std::vector<int>> towns;
    int optimal_result;

public:
    int getOptimalResult() const;
    void loadDataFromFile(const char *filename);
    void printData();
    std::vector<std::vector<int>> getTowns();
};

#endif //PEA3_TOWNS_H
