//
// Created by Rafał Rodak on 14.01.2022.
//


#include <iostream>

#include "Towns.h"
#include "GeneticAlgorithm.h"
#include <iostream>

using namespace std;

void menu() {
    bool quit = false;
    char option;
    char filename[50];
    int stop_time = 60;
    int population = 100;
    float crossesRate = 0.8;
    float mutationsRate = 0.01;
    int dataReadCount = 0;
    int value;
    float valueFloat;

    Towns towns;
    TSP_Algorithm::CrossingOperations crossingOperations = TSP_Algorithm::PMX;
    TSP_Algorithm::MutationOperations mutationOperations = TSP_Algorithm::Insert;
    string name;
    do {
        cout << endl;
        cout << "================== MAIN MENU ==================" << endl;
        cout << "0 - Close program" << endl;
        cout << "1 - Load from the file" << endl;
        cout << "2 - Print loaded data" << endl;
        cout << "3 - Modify stop time, current = " << stop_time << endl;
        cout << "4 - Modify population size, current = " << population << endl;
        cout << "5 - Modify mutation rate, current = " << mutationsRate << endl;
        cout << "6 - Modify crossing rate, current = " << crossesRate << endl;
        cout << "7 - Choose crossing method, current = ";
        switch (crossingOperations)
        {
            case 1:
                cout << "Partially Mapped Crossover" << endl;
                break;
            case 2:
                cout << "Ordered Crossover" << endl;
                break;
        }
        cout << "8 - Choose mutation method, current = ";
        switch (mutationOperations)
        {
            case 1:
                cout << "Insert" << endl;
                break;
            case 2:
                cout << "Swap" << endl;
                break;
        }
        cout << "e - Execute algorithm" << endl;
        cout << "t - Execute tests" << endl;
        cout << "AG> ";
        cin >> option;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        switch (option) {
            case '0':
                quit = true;
                break;
            case '1':
                cout << "Enter file name: ";
                cin >> filename;
                cout << endl;
                towns.loadDataFromFile(filename);
                if (!towns.getTowns().empty() && dataReadCount == 0)
                {
                    int number_of_towns = towns.getTowns()[0].size();
                    dataReadCount ++;
                }
                break;
            case '2':
                if (towns.getTowns().empty())
                {
                    cout << "Load data first!" << endl;
                    break;
                }
                towns.printData();
                break;
            case '3': {
                cout << "Enter new stop criteria: ";
                cin >> value;
                if (value <= 0 || cin.fail())
                {
                    cout << "Invalid stop criteria!" << endl;
                    break;
                }
                stop_time = value;
                break;
            }
            case '4':
               cout << "Enter new population: ";
                cin >> value;
                if (value <= 0 || cin.fail())
                {
                    cout << "Invalid population!" << endl;
                    break;
                }
                population = value;
                break;
            case '5':
                cout << "Enter new mutation rate: ";
                cin >> valueFloat;
                if (valueFloat <= 0.0 || valueFloat>=1.0 ||cin.fail())
                {
                    cout << "Invalid mutation rate!" << endl;
                    break;
                }
                mutationsRate = valueFloat;
                break;
            case '6':{
                cout << "Enter new crossing rate: ";
                cin >> valueFloat;
                if (valueFloat <= 0.0 || valueFloat>=1.0 ||cin.fail())
                {
                    cout << "Invalid crossing rate!" << endl;
                    break;
                }
                crossesRate = valueFloat;
                break;
            }
            case '7':{
                cout << "1 - Partially Mapped Crossover" << endl;
                cout << "2 - Ordered Crossover" << endl;

                cout << "Enter new crossing method: ";
                cin >> value;
                if (value <= 0 || cin.fail() || value > 2)
                {
                    cout << "Invalid crossing method!" << endl;
                    break;
                }
                switch (value)
                {
                    case 1:
                        crossingOperations = TSP_Algorithm::PMX;
                        break;
                    case 2:
                        crossingOperations = TSP_Algorithm::OX;
                        break;
                }
                break;
            }
            case '8':{
                cout << "1 - Insert" << endl;
                cout << "2 - Swap" << endl;

                cout << "Enter new mutation method: ";
                cin >> value;
                if (value <= 0 || cin.fail() || value > 2)
                {
                    cout << "Invalid mutation method!" << endl;
                    break;
                }
                switch (value)
                {
                    case 1:
                        mutationOperations = TSP_Algorithm::Insert;
                        break;
                    case 2:
                        mutationOperations = TSP_Algorithm::Swap;
                        break;
                }
                break;
            }
            case 'e':{
                if(towns.getTowns().empty()){
                    cout << "Load data first!" << endl;
                    break;
                }
                GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations);
                geneticAlgorithm.apply();
                geneticAlgorithm.printBestRoute();
                break;
            }

            case 't': {
                stop_time = 30;
                population = 100;
                towns.loadDataFromFile("ftv55.txt");
                crossingOperations = TSP_Algorithm::PMX;
                mutationOperations = TSP_Algorithm::Insert;
                int i = 0;
                for ( ; ; ){
                    GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations);
                    geneticAlgorithm.apply();
                    geneticAlgorithm.printBestRoute();
                    i ++;
                    if(i==5)break;
                }
                crossingOperations = TSP_Algorithm::OX;
                cout << "\n\n";
                i = 0;
                for ( ; ; ){
                    GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations);
                    geneticAlgorithm.apply();
                    geneticAlgorithm.printBestRoute();
                    i ++;
                    if(i==5)break;
                }

                break;
            }
            default:
                cout << "Wrong option chosen.\n";
        }

    } while (!quit);
}

int main()
{
    menu();
    return 0;
}