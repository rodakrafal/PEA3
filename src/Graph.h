//
// Created by Rafał Rodak on 14.01.2022.
//

#ifndef PEA3_GRAPH_H
#define PEA3_GRAPH_H

#include <string>
#include <vector>

using namespace std;

class Graph {
public:
    vector<vector<int>> matrix{};
    int number_of_vertices;

    Graph();

    virtual ~Graph();

    void loadData(const string &name_file);

    void display() const;

};


#endif //PEA3_GRAPH_H
