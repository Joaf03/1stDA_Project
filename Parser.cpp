//
// Created by carolina on 17/03/25.
//

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Parser.h"

/**
 * @file Parser.cpp
 * @brief Implementation file for the Parser class.
 *
 * This file contains the implementation of the Parser class methods for parsing location and distance data from CSV files.
 */


/**
 * @brief Parses location data from a CSV file and adds vertices to the graph.
 *
 * @param filename The name of the CSV file containing location data.
 * @param graph The graph object to which the vertices will be added.
 */
void Parser::parseLocations(const string &filename, Graph<int> &graph) {
    cout << "Parsing locations from " << filename << endl;
    ifstream file(filename);
    string line;
    cout << line << endl;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string location, code;
        int id, parking;
        getline(ss, location, ',');
        ss >> id;
        ss.ignore();
        getline(ss, code, ',');
        ss >> parking;
        graph.addVertex(id, location, code, parking);
        graph.codeToId[code] = id;
    }
}

/**
* @brief Parses distance data from a CSV file and adds edges to the graph.
*
* @param filename The name of the CSV file containing distance data.
* @param graph The graph object to which the edges will be added.
*/
void Parser::parseDistances(const string &filename, Graph<int> &graph) {
    ifstream file(filename);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string loc1, loc2, driving, walking;
        getline(ss, loc1, ',');
        getline(ss, loc2, ',');
        getline(ss, driving, ',');
        getline(ss, walking, ',');

        int origInt = graph.codeToId[loc1];
        int destInt = graph.codeToId[loc2];

        Vertex<int> * orig = graph.findVertex(origInt);
		Vertex<int> * dest = graph.findVertex(destInt);

        int drivingTime = (driving == "X") ? 500000 : stoi(driving);
        int walkingTime = stoi(walking);
        graph.addEdge(orig, dest, drivingTime, walkingTime);
    }
}