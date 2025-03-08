//
// Created by curlmike on 08-03-2025.
//

#include "Menu.h"
#include <iostream>

template <class T>
void Menu<T>::fastestIndependantRoute(Graph<T> *g) {
    // change findVertex to work with strings
    string source, target;
    Vertex<T>* sourceVertex, targetVertex;
    bool validSource = false, validTarget = false;

    while (!validSource) {
        std::cout << "Please enter the source vertex: ";
        std::cin >> source;
        if ((sourceVertex = g->findVertex(source)) != nullptr) {
            validSource = true;
        } else {
            std::cout << "Invalid source vertex. Please try again: ";
        }
    }

    while (!validTarget) {
        std::cout << "Please enter the target vertex: ";
        std::cin >> target;
        if ((targetVertex = g->findVertex(target)) != nullptr) {
            validTarget = true;
        } else {
            std::cout << "Invalid target vertex. Please try again: ";
        }
    }

    printFastestIndependantRoute(g, sourceVertex, targetVertex);
}

template <class T>
void Menu<T>::fastestRestrictedRoute(Graph<T> *g) {
    // change findVertex to work with strings
    std::vector<Vertex<T>*> nodesToAvoid = {}, segmentsToAvoid = {};
    string source, target, includeNode;
    Vertex<T>* sourceVertex, targetVertex, includeVertex;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validIncludeNode = false;

    while (!validSource) {
        std::cout << "Please enter the source vertex: ";
        std::cin >> source;
        if ((sourceVertex = g->findVertex(source)) != nullptr) {
            validSource = true;
        } else {
            std::cout << "Invalid source vertex. Please try again: ";
        }
    }

    while (!validTarget) {
        std::cout << "Please enter the target vertex: ";
        std::cin >> target;
        if ((targetVertex = g->findVertex(target)) != nullptr) {
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

    while (!validIncludeNode) {
        std::cout << "Please enter the node to include (leave blank for no include node): ";
        std::cin >> includeNode;
        if (includeNode.empty()) break;
        if ((includeNode = g->findVertex(includeNode)) != nullptr) {
            validIncludeNode = true;
        } else {
            std::cout << "Invalid node " << includeNode << ". Please try again: ";
        }
    }

    printFastestRestrictedRoute(g, sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid, includeNode);
}

template <class T>
void Menu<T>::printFastestIndependantRoute(Graph<T> *g, Vertex<T>* source, Vertex<T>* target) {
    // Implement the logic to find and print the fastest independent route
    // This is a placeholder implementation
    std::cout << "Fastest Independent Route from " << source << " to " << target << std::endl;
}

template <class T>
void Menu<T>::printFastestRestrictedRoute(Graph<T> *g, Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
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
                this->fastestIndependantRoute(this->getGraph());
                exit = true;
            break;
            case 2:
                this->fastestRestrictedRoute(this->getGraph());
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