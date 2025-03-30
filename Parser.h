//
// Created by carolina on 17/03/25.
//

#ifndef PARSER_H
#define PARSER_H

#include "./data_structures/Graph.h"

using namespace std;


class Parser {
    public:
        static void parseLocations(const std::string &filename, Graph<int> &graph);
        static void parseDistances(const std::string &filename, Graph<int> &graph);

};



#endif //PARSER_H
