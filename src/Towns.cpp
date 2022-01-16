//
// Created by Rafał Rodak on 15.01.2022.
//

#include "Towns.h"
#include <fstream>
#include <iostream>

using namespace std;

int Towns::getOptimalResult() const
{
    return optimal_result;
}

void Towns::loadDataFromFile(const char *filename)
{
    ifstream file;
    file.open(filename);

    if (!file)
    {
        cout << "File open error" << endl;
        return;
    }
    else
    {
        int temp;
        file >> temp; // liczba miast
        if (temp <= 0)
        {
            cout << "The number of towns is incorrect" << endl;
            return;
        }
        if (file.fail())
        {
            cout << "Wrong data in file" << endl;
            return;
        }
        // vector wypełniony taką ilością vectorów, z wartością 0, ile jest miast
        vector<vector<int>> temp_towns(temp, vector<int>(temp, 0));
        // w kolejnych liniach odległości z danego miasta do wszystkich pozostałych miast
        for (int i = 0; i < temp; i++)
        {
            for (int j = 0; j < temp; j++)
            {
                file >> temp_towns[i][j];
                if (file.fail())
                {
                    cout << "Wrong data in file" << endl;
                    return;
                }
                if (file.peek() == EOF && j != temp - 1)
                {
                    cout << "Not enough data compared to the number of towns" << endl;
                    return;
                }
            }
        }
        towns = temp_towns;
        file.close();
        for (int i = 0; i < temp; i++)
            towns[i][i] = -1;
    }
    optimal_result = 0;
    if (strcmp(filename, "ftv170.tsp") == 0)
        optimal_result = 2755;
    else if (strcmp(filename, "rbg403.tsp") == 0)
        optimal_result = 2465;
    else if (strcmp(filename, "ftv47.tsp") == 0)
        optimal_result = 1776;
    else if (strcmp(filename, "ftv55.txt") == 0)
        optimal_result = 1608;
    cout << "Data loaded successfully" << endl;
}
void Towns::printData()
{
    for (const auto& x : towns)
    {
        for (int y : x)
            cout << y << "\t";
        cout << endl;
    }
}

vector<vector<int>> Towns::getTowns()
{
    return towns;
}