//
// Created by Rafał Rodak on 14.01.2022.
//


#include <iostream>

#include "Towns.h"
#include "GeneticAlgorithm.h"

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
                GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations,0);
                geneticAlgorithm.apply(3);
                geneticAlgorithm.printBestRoute();
                if (towns.getOptimalResult() != 0)
                    cout << "Relative error: " << float(abs(geneticAlgorithm.getBestRouteCost() - towns.getOptimalResult())) / towns.getOptimalResult() * 100 << "%" << endl;
                break;
            }

            case 't': {
                // ------------------------------------ PMX INSERT
                int tmp = 0, currentFile = 0,i;
                population = 1000;
                crossesRate = 0.8;
                mutationsRate = 0.05;

                crossingOperations = TSP_Algorithm::PMX;
                mutationOperations = TSP_Algorithm::Insert;

                for(;;){
                    if(currentFile == 0){
                        towns.loadDataFromFile("ftv47.tsp");
                        stop_time = 60;
                    }
                    if(currentFile == 1){
                        towns.loadDataFromFile("ftv170.tsp");
                        stop_time = 120;

                    }
                    if(currentFile == 2){
                        towns.loadDataFromFile("rbg403.tsp");
                        stop_time = 180;
                    }

                    for (;;){
                        i = 0;
                        for ( ; ; ){
                            cout << "Rozmiar: " <<to_string(towns.getTowns()[0].size()) << " ";
                            cout << "PMX INSERT iteracja: " << i+1 << endl;
                            cout << "Populacja: " << population << endl;
                            cout << "Mutacja: " << mutationsRate << endl;
                            GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations,i+1);
                            geneticAlgorithm.apply(tmp);
                            i ++;
                            if(i==2)break;
                        }
                        tmp ++;
                        if(tmp == 2) break;
                        if(tmp == 1) mutationsRate = 0.1;
                    }
                    tmp = 0;
                    mutationsRate = 0.01;
                    if(currentFile==2)break;
                    currentFile++;
                }
                // ------------------------------------ PMX Swap
                tmp = 0, currentFile = 0;
                mutationsRate = 0.05;

                crossingOperations = TSP_Algorithm::PMX;
                mutationOperations = TSP_Algorithm::Swap;

                for(;;){
                    if(currentFile == 0){
                        towns.loadDataFromFile("ftv47.tsp");
                        stop_time = 60;
                    }
                    if(currentFile == 1){
                        towns.loadDataFromFile("ftv170.tsp");
                        stop_time = 120;

                    }
                    if(currentFile == 2){
                        towns.loadDataFromFile("rbg403.tsp");
                        stop_time = 180;
                    }

                    for (;;){
                        i = 0;
                        for ( ; ; ){
                            cout << "Rozmiar: " <<to_string(towns.getTowns()[0].size()) << " ";
                            cout << "PMX Swap iteracja: " << i+1 << endl;
                            cout << "Populacja: " << population << endl;
                            cout << "Mutacja: " << mutationsRate << endl;
                            GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations,i+1);
                            geneticAlgorithm.apply(tmp);
                            i ++;
                            if(i==2)break;
                        }
                        tmp ++;
                        if(tmp == 2) break;
                        if(tmp == 1) mutationsRate = 0.1;
                    }
                    tmp = 0;
                    mutationsRate = 0.01;
                    if(currentFile==2)break;
                    currentFile++;
                }

                // ------------------------------------ OX INSERT
                tmp = 0, currentFile = 0;
                mutationsRate = 0.05;

                crossingOperations = TSP_Algorithm::OX;
                mutationOperations = TSP_Algorithm::Insert;

                for(;;){
                    if(currentFile == 0){
                        towns.loadDataFromFile("ftv47.tsp");
                        stop_time = 60;
                    }
                    if(currentFile == 1){
                        towns.loadDataFromFile("ftv170.tsp");
                        stop_time = 120;

                    }
                    if(currentFile == 2){
                        towns.loadDataFromFile("rbg403.tsp");
                        stop_time = 180;
                    }

                    for (;;){
                        i = 0;
                        for ( ; ; ){
                            cout << "Rozmiar: " <<to_string(towns.getTowns()[0].size()) << " ";
                            cout << "OX INSERT iteracja: " << i+1 << endl;
                            cout << "Populacja: " << population << endl;
                            cout << "Mutacja: " << mutationsRate << endl;
                            GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations,i+1);
                            geneticAlgorithm.apply(tmp);
                            i ++;
                            if(i==2)break;
                        }
                        tmp ++;
                        if(tmp == 2) break;
                        if(tmp == 1) mutationsRate = 0.1;
                    }
                    tmp = 0;
                    mutationsRate = 0.01;
                    if(currentFile==2)break;
                    currentFile++;
                }

                // ------------------------------------ OX Swap
                tmp = 0, currentFile = 0;
                mutationsRate = 0.05;

                crossingOperations = TSP_Algorithm::OX;
                mutationOperations = TSP_Algorithm::Swap;

                for(;;){
                    if(currentFile == 0){
                        towns.loadDataFromFile("ftv47.tsp");
                        stop_time = 60;
                    }
                    if(currentFile == 1){
                        towns.loadDataFromFile("ftv170.tsp");
                        stop_time = 120;

                    }
                    if(currentFile == 2){
                        towns.loadDataFromFile("rbg403.tsp");
                        stop_time = 180;
                    }

                    for (;;){
                        i = 0;
                        for ( ; ; ){
                            cout << "Rozmiar: " <<to_string(towns.getTowns()[0].size()) << " ";
                            cout << "OX Swap iteracja: " << i+1 << endl;
                            cout << "Populacja: " << population << endl;
                            cout << "Mutacja: " << mutationsRate << endl;
                            GeneticAlgorithm geneticAlgorithm(towns.getTowns(),stop_time,population,crossesRate,mutationsRate, crossingOperations, mutationOperations,i+1);
                            geneticAlgorithm.apply(tmp);
                            i ++;
                            if(i==2)break;
                        }
                        tmp ++;
                        if(tmp == 2) break;
                        if(tmp == 1) mutationsRate = 0.1;
                    }
                    tmp = 0;
                    mutationsRate = 0.01;
                    if(currentFile==2)break;
                    currentFile++;
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