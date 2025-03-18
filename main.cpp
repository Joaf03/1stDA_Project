#include "./data_structures/Graph.h"
#include "Parser.h"
#include "Menu.h"
#include <iostream>


int main() {
    // Create a Graph object
    Graph<int> graph;
    Menu<int> menu(&graph);

    // Parse the Locations.csv file
    Parser::parseLocations("../data/Locations.csv", graph);

    // Parse the Distances.csv file
    Parser::parseDistances("../data/Distances.csv", graph);

    // Display the parsed vertices
    cout << "Vertices:" << endl;
    for (const auto& vertex : graph.getVertexSet()) {
        cout << "ID: " << vertex->getInfo()
             << ", Code: " << vertex->getCode()
             << ", Location: " << vertex->getLocation()
             << ", Parking: " << (vertex->getParking() ? "Yes" : "No")
             << endl;
    }

    // Display the parsed edges
    cout << "Edges:" << endl;
    for (const auto& edge : graph.getEdges()) {
        cout << "Origin: " << edge->getOrig()->getCode()
             << ", Destination: " << edge->getDest()->getCode()
             << ", Driving Time: " << edge->getDrivingTime()
             << ", Walking Time: " << edge->getWalkingTime()
             << endl;
    }

    menu.printInitialMenu();

    return 0;
}