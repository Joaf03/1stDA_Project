//
// Created by curlmike on 08-03-2025.
//

#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <sstream>
#include <vector>
#include "dijkstra.h"

using namespace std;

template <class T>
class Menu {
public:
    explicit Menu(Graph<T>* g): g(g) {}

    void printInitialMenu();

    void fastestIndependantRoute();

    void fastestRestrictedRoute();

    void printFastestIndependantRoute(Vertex<T>* source, Vertex<T>* target);

    void printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid);

    void printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid, Vertex<T>* includeNode);

    Graph<T>* getGraph() const {
        return g;
    }

protected:
    Graph<T>* g;
};

template <class T>
void Menu<T>::fastestIndependantRoute() {
    string source, target;
    bool validSource = false, validTarget = false;

    while (!validSource) {
        std::cout << "Please enter the source vertex: ";
        std::cin >> source;
        Vertex<T>* sourceVertex = g->findVertex(source);
        if (sourceVertex != nullptr) {
            validSource = true;
        } else {
            std::cout << "Invalid source vertex. Please try again: ";
        }
    }

    while (!validTarget) {
        std::cout << "Please enter the target vertex: ";
        std::cin >> target;
        Vertex<T>* targetVertex = g->findVertex(target);
        if (targetVertex != nullptr) {
            validTarget = true;
        } else {
            std::cout << "Invalid target vertex. Please try again: ";
        }
    }

    Vertex<T>* sourceVertex = g->findVertex(source);
    Vertex<T>* targetVertex = g->findVertex(target);

    printFastestIndependantRoute(sourceVertex, targetVertex);
}

template <class T>
void Menu<T>::fastestRestrictedRoute() {
    // change findVertex to work with strings
    std::vector<Vertex<T>*> nodesToAvoid = {};
    std::vector<Edge<T>*> segmentsToAvoid = {};
    string source, target, includeNode;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validIncludeNode = false;

    while (!validSource) {
        std::cout << "Please enter the source vertex: ";
        std::cin >> source;
        Vertex<T>* sourceVertex = g->findVertex(source);
        if (sourceVertex != nullptr) {
            validSource = true;
        } else {
            std::cout << "Invalid source vertex. Please try again: ";
        }
    }

    while (!validTarget) {
        std::cout << "Please enter the target vertex: ";
        std::cin >> target;
        Vertex<T>* targetVertex = g->findVertex(target);
        if (targetVertex != nullptr) {
            validTarget = true;
        } else {
            std::cout << "Invalid target vertex. Please try again: ";
        }
    }

    while (!validAvoidNodes) {
        bool invalidNodes = false;
        std::cout << "Please enter the nodes to avoid (separated by commas, leave blank for no nodes): ";
        std::string nodes;
        std::cin >> nodes;
        std::stringstream ss(nodes);
        std::string node;
        while (std::getline(ss, node, ',')) {
            Vertex<T>* vertex = g->findVertex(node);
            if (vertex != nullptr) {
                nodesToAvoid.push_back(vertex);
            } else {
                std::cout << "Invalid node " << node << ". Please try again: ";
                nodesToAvoid.clear();
                invalidNodes = true;
                break;
            }
        }
        if (!invalidNodes) validAvoidNodes = true;
    }

    /*
    TODO: FIX
    while (!validAvoidSegments) {
        bool invalidSegments = false;
        std::cout << "Please enter the segments to avoid (separated by commas, leave blank for no segments): ";
        std::string segments;
        std::cin >> segments;
        std::stringstream ss(segments);
        std::string segment;
        if (segments.empty()) break;
        while (std::getline(ss, segment, ',')) {
            Edge<T>* edge = g->findEdge(segment);
            if (edge != nullptr) {
                segmentsToAvoid.push_back(edge);
            } else {
                std::cout << "Invalid segment " << segment << ". Please try again: ";
                segmentsToAvoid.clear();
                invalidSegments = true;
                break;
            }
        }
        if (!invalidSegments) validAvoidSegments = true;
    }
    */

    while (!validIncludeNode) {
        std::cout << "Please enter the node to include (leave blank for no include node): ";
        std::cin >> includeNode;
        if (includeNode.empty()) break;
        Vertex<T>* includeVertex = g->findVertex(includeNode);
        if (includeVertex != nullptr) {
            validIncludeNode = true;
        } else {
            std::cout << "Invalid node " << includeNode << ". Please try again: ";
        }
    }

    Vertex<T>* sourceVertex = g->findVertex(source);
    Vertex<T>* targetVertex = g->findVertex(target);

    if (validIncludeNode) {
        Vertex<T>* includeVertex = g->findVertex(includeNode);
        printFastestRestrictedRoute(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid, includeVertex);
    } else {
        printFastestRestrictedRoute(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid);
    }
}

template <class T>
void Menu<T>::printFastestIndependantRoute(Vertex<T>* source, Vertex<T>* target) {

    std::vector<Vertex<T>*> avoidNodes = {};

    dijkstra(g, source->getLocation(), {}, {});

    std::vector<string> path = getPath(g, source->getLocation(), target->getLocation());

    for (int i = 1; i < path.size() - 1; i++) {
        avoidNodes.push_back(g->findVertex(path[i]));
    }

    std::cout << "Fastest Independent Route from " << source << " to " << target << std::endl;

    for (const string& location : path) {
        std::cout << location << "->";
    }

    dijkstra(g, source->getLocation(), avoidNodes, {});

    std::vector<string> path2 = getPath(g, source->getLocation(), target->getLocation());

    std::cout << "Fastest Alternative Independent Route from " << source << " to " << target << std::endl;

    for (const string& location : path2) {
        std::cout << location << "->";
    }
}

template <class T>
void Menu<T>::printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
    std::vector<Edge<T>*> segmentsToAvoid) {
    // Implement the logic to find and print the fastest restricted route
    // This is a placeholder implementation
    std::cout << "Fastest Restricted Route from " << source << " to " << target << std::endl;
}

template <class T>
void Menu<T>::printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
    std::vector<Edge<T>*> segmentsToAvoid, Vertex<T>* includeNode) {
    // Implement the logic to find and print the fastest restricted route
    // This is a placeholder implementation
    std::cout << "Fastest Restricted Route from " << source << " to " << target << std::endl;
}

template <class T>
void Menu<T>::printInitialMenu() {
    unsigned int option;
    bool exit = false;

    std::cout << "1. Find the fastest independent route\n";
    std::cout << "2. Find the fastest restricted route\n";
    std::cout << "3. Exit\n";

    while (!exit) {

        std::cout << "Please choose an option: ";
        std::cin >> option;
        std::cout << std::endl;

        switch (option) {
            case 1:
                this->fastestIndependantRoute();
                exit = true;
            break;
            case 2:
                this->fastestRestrictedRoute();
                exit = true;
            break;
            case 3:
                std::cout << "Exiting...\n";
                exit = true;
            break;
            default:
                std::cout << "Invalid option. Please try again.\n";
            break;
        }
    }
}

#endif //MENU_H
