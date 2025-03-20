//
// Created by curlmike on 08-03-2025.
//

#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>

#include "dijkstra.h"

using namespace std;

template <class T>
class Menu {
public:
    explicit Menu(Graph<T>* g): g(g) {}

    void printInitialMenu();

    void fastestIndependantRoute();

    void fastestRestrictedRoute();

    void fastestEnvFriendlyRoute();

    void printFastestIndependantRoute(Vertex<T>* source, Vertex<T>* target);

    void printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid);

    void printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid, Vertex<T>* includeNode);

    void printFastestEnvFriendlyRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
                                     std::vector<Edge<T>*> segmentsToAvoid);

    void printFastestEnvFriendlyRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
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

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (!validSource) {
        std::cout << "Please enter the source vertex: ";
        std::getline(std::cin, source);
        Vertex<T>* sourceVertex = g->findVertex(source);
        if (sourceVertex != nullptr) {
            validSource = true;
        } else {
            std::cout << "\nInvalid source vertex. Please try again: ";
        }
    }

    while (!validTarget) {
        std::cout << "Please enter the target vertex: ";
        std::getline(std::cin, target);
        Vertex<T>* targetVertex = g->findVertex(target);
        if (targetVertex != nullptr) {
            validTarget = true;
        } else {
            std::cout << "\nInvalid target vertex. Please try again: ";
        }
    }

    Vertex<T>* sourceVertex = g->findVertex(source);
    Vertex<T>* targetVertex = g->findVertex(target);

    printFastestIndependantRoute(sourceVertex, targetVertex);
}

template <class T>
void Menu<T>::fastestRestrictedRoute() {
    std::vector<Vertex<T>*> nodesToAvoid = {};
    std::vector<Edge<T>*> segmentsToAvoid = {};
    string source, target, includeNode;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validIncludeNode = false;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (!validSource) {
        std::cout << "Please enter the source vertex: ";
        std::getline(std::cin, source);
        Vertex<T>* sourceVertex = g->findVertex(source);
        if (sourceVertex != nullptr) {
            validSource = true;
        } else {
            std::cout << "\nInvalid source vertex. Please try again: ";
        }
    }

    while (!validTarget) {
        std::cout << "Please enter the target vertex: ";
        std::getline(std::cin, target);
        Vertex<T>* targetVertex = g->findVertex(target);
        if (targetVertex != nullptr) {
            validTarget = true;
        } else {
            std::cout << "\nInvalid target vertex. Please try again: ";
        }
    }

    while (!validAvoidNodes) {
        bool invalidNodes = false;
        std::cout << "Please enter the nodes to avoid (separated by commas, type 'none' for no nodes): ";
        std::string nodes;
        std::getline(std::cin, nodes);
        if (nodes == "none") {
            validAvoidNodes = true;
            break;
        }
        std::stringstream ss(nodes);
        std::string node;
        while (std::getline(ss, node, ',')) {
            Vertex<T>* vertex = g->findVertex(node);
            if (vertex != nullptr) {
                nodesToAvoid.push_back(vertex);
            } else {
                std::cout << "\nInvalid node " << node << ". Please try again: ";
                nodesToAvoid.clear();
                invalidNodes = true;
                break;
            }
        }
        if (!invalidNodes) validAvoidNodes = true;
    }

    while (!validAvoidSegments) {
        bool invalidSegments = false;
        std::cout << "Please enter the segments to avoid (in the format of ('location', 'location'), type 'none' for no segments): ";
        std::string segments;
        std::getline(std::cin, segments);
        if (segments == "none") {
            validAvoidSegments = true;
            break;
        }
        std::stringstream ss(segments);
        std::string segment;
        if (segments.empty()) break;
        while (std::getline(ss, segment, ')')) {
            if (segment.front() == '(') {
                segment = segment.substr(1);
            }
            if (segment.back() == ',') {
                segment.pop_back();
            }
            std::string srcLocation = segment.substr(0, segment.find(','));
            std::string dstLocation = segment.substr(segment.find(',') + 1);
            Vertex<T>* srcVertex = g->findVertex(srcLocation);
            Vertex<T>* dstVertex = g->findVertex(dstLocation);
                if (srcVertex != nullptr && dstVertex != nullptr) {
                    Edge<T>* edge = g->findEdge(srcVertex->getLocation(), dstVertex->getLocation());
                    if (edge != nullptr) {
                        segmentsToAvoid.push_back(edge);
                    } else {
                        std::cout << "\nInvalid segment " << segment << ". Please try again: ";
                        std::cout << std::endl;
                        segmentsToAvoid.clear();
                        invalidSegments = true;
                        break;
                    }
                } else {
                    std::cout << "\nInvalid segment " << segment << ". Please try again: ";
                    std::cout << std::endl;
                    segmentsToAvoid.clear();
                    invalidSegments = true;
                    break;
                }
            }
        if (!invalidSegments) validAvoidSegments = true;
        }


    while (!validIncludeNode) {
        std::cout << "Please enter the node to include (type 'none' for no include node): ";
        std::getline(std::cin, includeNode);
        if (includeNode == "none") break;
        Vertex<T>* includeVertex = g->findVertex(includeNode);
        if (includeVertex != nullptr) {
            validIncludeNode = true;
        } else {
            std::cout << "\nInvalid node " << includeNode << ". Please try again: ";
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

    dijkstra(g, source->getLocation(), {}, {}, true);

    std::pair<std::vector<string>, int> res = getPath(g, source->getLocation(), target->getLocation());

    for (int i = 1; i < res.first.size() - 1; i++) {
        avoidNodes.push_back(g->findVertex(res.first[i]));
    }

    std::cout << "Fastest Independent Route from " << source->getLocation() << " to " << target->getLocation() << std::endl;

    for (size_t i = 0; i < res.first.size(); ++i) {
        std::cout << res.first[i];
        if (i != res.first.size() - 1) {
            std::cout << " -> ";
        }
    }

    std::cout << "\nTotal distance: " << res.second << std::endl << std::endl;

    for (int i = 1; i < res.first.size() - 1; i++) {
        avoidNodes.push_back(g->findVertex(res.first[i]));
    }

    dijkstra(g, source->getLocation(), avoidNodes, {}, true);

    std::pair<std::vector<string>, int> res2 = getPath(g, source->getLocation(), target->getLocation());

    std::cout << "Fastest Alternative Independent Route from " << source->getLocation() << " to " << target->getLocation() << std::endl;

    for (size_t i = 0; i < res2.first.size(); ++i) {
        std::cout << res2.first[i];
        if (i != res2.first.size() - 1) {
            std::cout << " -> ";
        }
    }

    std::cout << "\nTotal distance: " << res2.second << std::endl;
}

template <class T>
void Menu<T>::printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
    std::vector<Edge<T>*> segmentsToAvoid) {

    dijkstra(g, source->getLocation(), nodesToAvoid, segmentsToAvoid, true);

    std::pair<std::vector<string>, int> res = getPath(g, source->getLocation(), target->getLocation());

    std::cout << "Fastest Restricted Route from " << source->getLocation() << " to " << target->getLocation() << std::endl;

    for (size_t i = 0; i < res.first.size(); ++i) {
        std::cout << res.first[i];
        if (i != res.first.size() - 1) {
            std::cout << " -> ";
        }
    }

    std::cout << "\nTotal distance: " << res.second << std::endl;
}

template <class T>
void Menu<T>::printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
    std::vector<Edge<T>*> segmentsToAvoid, Vertex<T>* includeNode) {
    int totaldist = 0;

    dijkstra(g, source->getLocation(), nodesToAvoid, segmentsToAvoid, true);

    std::pair<std::vector<string>, int> res = getPath(g, source->getLocation(), includeNode->getLocation());
    totaldist += res.second;

    dijkstra(g, includeNode->getLocation(), nodesToAvoid, segmentsToAvoid, true);

    std::pair<std::vector<string>, int> secondPath = getPath(g, includeNode->getLocation(), target->getLocation());
    totaldist += secondPath.second;

    for (size_t i = 1; i < secondPath.first.size(); ++i) {
        res.first.push_back(secondPath.first[i]);
    }

    std::cout << "Fastest Restricted Route from " << source->getLocation() << " to " << includeNode->getLocation() << std::endl;

    for (size_t i = 0; i < res.first.size(); ++i) {
        std::cout << res.first[i];
        if (i != res.first.size() - 1) {
            std::cout << " -> ";
        }
    }

    std::cout << "\nTotal distance: " << totaldist << std::endl;
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
