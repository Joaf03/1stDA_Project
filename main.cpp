#include "Graph.h"
#include <iostream>

using namespace std;

int main() {
    // Create a Graph object
    Graph<int> graph;

    // Parse the Locations.csv file
    graph.parseLocations("For%20Students/Locations.csv");

    // Parse the Distances.csv file
    graph.parseDistances("For%20Students/Distances.csv");

    // Display the parsed vertices
    cout << "Vertices:" << endl;
    for (const auto& vertex : graph.getVertices()) {
        cout << "ID: " << vertex->getID()
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

    return 0;
}