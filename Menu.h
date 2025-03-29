//
// Created by curlmike on 08-03-2025.
//

/**
 * @file Menu.h
 * @brief Header file for the Menu class.
 *
 * This file contains the declaration of the Menu class and its member functions.
 */

#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>

#include "dijkstra.h"

using namespace std;


/**
 * @class Menu
 * @brief A class to represent the menu for the graph operations.
 *
 * This class provides various functions to interact with the graph and perform different types of route calculations.
 *
 * @tparam T The type of the vertex information.
 */

template <class T>
class Menu {
public:
    /**
     * @brief Constructor for the Menu class.
     * @param g Pointer to the graph object.
     * @param batch_mode Flag to indicate if the menu is in batch mode.
     */
    explicit Menu(Graph<T>* g, bool batch_mode = false) : g(g), batch_mode(batch_mode) {}

    /**
     * @brief Prints the initial menu options.
     */
    void printInitialMenu();

    /**
     * @brief Finds the fastest independent route.
     */
    void fastestIndependantRoute();

    /**
     * @brief Finds the fastest restricted route.
     */
    void fastestRestrictedRoute();

    /**
     * @brief Finds the fastest environmentally friendly route.
     */
    void fastestEnvFriendlyRoute();

    /**
     * @brief Finds alternative environmentally friendly routes.
     */
    void alternativeEnvFriendlyRouts();

    /**
     * @brief Prints the fastest independent route.
     * @param source Pointer to the source vertex.
     * @param target Pointer to the target vertex.
     */
    void printFastestIndependantRoute(Vertex<T>* source, Vertex<T>* target);

    /**
     * @brief Prints the fastest restricted route.
     * @param source Pointer to the source vertex.
     * @param target Pointer to the target vertex.
     * @param nodesToAvoid Vector of nodes to avoid.
     * @param segmentsToAvoid Vector of segments to avoid.
     */
    void printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid);

    /**
     * @brief Prints the fastest restricted route with an included node.
     * @param source Pointer to the source vertex.
     * @param target Pointer to the target vertex.
     * @param nodesToAvoid Vector of nodes to avoid.
     * @param segmentsToAvoid Vector of segments to avoid.
     * @param includeNode Pointer to the node to include.
     */
    void printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid, Vertex<T>* includeNode);

    /**
     * @brief Prints the fastest environmentally friendly route.
     * @param source Pointer to the source vertex.
     * @param target Pointer to the target vertex.
     * @param nodesToAvoid Vector of nodes to avoid.
     * @param segmentsToAvoid Vector of segments to avoid.
     * @param maxWalkingTime Maximum walking time.
     */
    void printFastestEnvFriendlyRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
                                     std::vector<Edge<T>*> segmentsToAvoid, int maxWalkingTime);

    /**
     * @brief Prints alternative environmentally friendly routes.
     * @param source Pointer to the source vertex.
     * @param target Pointer to the target vertex.
     * @param nodesToAvoid Vector of nodes to avoid.
     * @param segmentsToAvoid Vector of segments to avoid.
     * @param maxWalkingTime Maximum walking time.
     */
    void printAlternativeEnvFriendlyRoutes(Vertex<T> *source, Vertex<T> *target, vector<Vertex<T> *> nodesToAvoid,
                                           vector<Edge<T> *> segmentsToAvoid, int maxWalkingTime);

    /**
     * @brief Returns the graph object.
     * @return Pointer to the graph object.
     */
    Graph<T>* getGraph() const {
        return g;
    }

protected:
    Graph<T>* g; ///< Pointer to the graph object.
    bool batch_mode; ///< Flag to indicate if the menu is in batch mode.
};

template <class T>
void Menu<T>::fastestIndependantRoute() {
    string source, target;
    int sourceID = -1, targetID = -1;
    bool validSource = false, validTarget = false;

    if (batch_mode) {
        std::ifstream inputFile("../input.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open input.txt\n";
            return;
        }
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, ':') && std::getline(iss, value)) {
                value.erase(0, value.find_first_not_of(" \t"));
                if (key == "Source") {
                    sourceID = stoi(value);
                } else if (key == "Destination") {
                    targetID = stoi(value);
                }
            }
        }
        inputFile.close();

        if (sourceID == -1 || targetID == -1) {
            std::cerr << "Error: Invalid input format in input.txt\n";
            return;
        }
    }
    else {
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
    }

    Vertex<T>* sourceVertex = nullptr;
    Vertex<T>* targetVertex = nullptr;

    if (batch_mode) {
        sourceVertex = g->findVertex(sourceID);
        targetVertex = g->findVertex(targetID);
    } else {
        sourceVertex = g->findVertex(source);
        targetVertex = g->findVertex(target);
    }

    printFastestIndependantRoute(sourceVertex, targetVertex);
}

template <class T>
void Menu<T>::fastestRestrictedRoute() {
    std::vector<Vertex<T>*> nodesToAvoid = {};
    std::vector<Edge<T>*> segmentsToAvoid = {};
    std::string source, target, includeNode;
    int sourceID = -1, targetID = -1, includeNodeID = -1;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validIncludeNode = false;

    if (batch_mode) {
        std::ifstream inFile("../input.txt");
        if (!inFile) {
            std::cerr << "Error opening input.txt" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string key, value;
            std::getline(ss, key, ':');

            if (key == "Source") {
                std::getline(ss, value);
                sourceID = std::stoi(value);
                validSource = (g->findVertex(sourceID) != nullptr);
            }
            else if (key == "Destination") {
                std::getline(ss, value);
                targetID = std::stoi(value);
                validTarget = (g->findVertex(targetID) != nullptr);
            }
            else if (key == "AvoidNodes") {
                std::string nodes;
                std::getline(ss, nodes);
                if (nodes != "none") {
                    std::stringstream ns(nodes);
                    std::string node;
                    while (std::getline(ns, node, ',')) {
                        Vertex<T>* vertex = g->findVertex(std::stoi(node));
                        if (vertex) {
                            nodesToAvoid.push_back(vertex);
                        } else {
                            std::cerr << "Invalid node to avoid: " << node << std::endl;
                        }
                    }
                }
            }
            else if (key == "AvoidSegments") {
                std::string segments;
                std::getline(ss, segments);
                if (segments != "none") {
                    std::stringstream segStream(segments);
                    std::string segment;
                    while (std::getline(segStream, segment, ')')) {
                        if (segment.front() == '(') segment = segment.substr(1);
                        if (segment.back() == ',') segment.pop_back();

                        size_t commaPos = segment.find(',');
                        if (commaPos != std::string::npos) {
                            std::string srcLocation = segment.substr(0, commaPos);
                            std::string dstLocation = segment.substr(commaPos + 1);

                            Vertex<T>* srcVertex = g->findVertex(std::stoi(srcLocation));
                            Vertex<T>* dstVertex = g->findVertex(std::stoi(dstLocation));
                            if (srcVertex && dstVertex) {
                                Edge<T>* edge = g->findEdge(srcVertex->getLocation(), dstVertex->getLocation());
                                if (edge) {
                                    segmentsToAvoid.push_back(edge);
                                } else {
                                    std::cerr << "Invalid segment: (" << srcLocation << "," << dstLocation << ")" << std::endl;
                                }
                            }
                        }
                    }
                }
            }
            else if (key == "IncludeNode") {
                std::getline(ss, includeNode);
                if (includeNode != "none") {
                    includeNodeID = std::stoi(includeNode);
                    validIncludeNode = (g->findVertex(includeNodeID) != nullptr);
                }
            }
        }
        inFile.close();
    }
    else {
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
    }

    Vertex<T>* sourceVertex = nullptr;
    Vertex<T>* targetVertex = nullptr;
    Vertex<T>* includeVertex = nullptr;

    if (batch_mode) {
        sourceVertex = g->findVertex(sourceID);
        targetVertex = g->findVertex(targetID);
        if (validIncludeNode) includeVertex = g->findVertex(includeNodeID);
    } else {
        sourceVertex = g->findVertex(source);
        targetVertex = g->findVertex(target);
        if (validIncludeNode) includeVertex = g->findVertex(includeNode);
    }

    if (validIncludeNode) {
        printFastestRestrictedRoute(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid, includeVertex);
    } else {
        printFastestRestrictedRoute(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid);
    }
}

template <class T>
void Menu<T>::fastestEnvFriendlyRoute() {
    std::vector<Vertex<T>*> nodesToAvoid = {};
    std::vector<Edge<T>*> segmentsToAvoid = {};
    int sourceID = -1, targetID = -1;
    string source, target;
    int maxWalkingTime;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validMaxWalkingTime = false;

        if (batch_mode) {
            std::ifstream inFile("../input.txt");
            if (!inFile) {
                std::cerr << "Error opening input.txt" << std::endl;
                return;
            }

            std::string line;
            while (std::getline(inFile, line)) {
                if (line.rfind("Source:", 0) == 0) {
                    source = line.substr(7);
                    sourceID = std::stoi(source);
                } else if (line.rfind("Destination:", 0) == 0) {
                    target = line.substr(12);
                    targetID = std::stoi(target);
                } else if (line.rfind("MaxWalkTime:", 0) == 0) {
                    maxWalkingTime = std::stoi(line.substr(12));
                } else if (line.rfind("AvoidNodes:", 0) == 0) {
                    std::string nodes = line.substr(11);
                    std::stringstream ss(nodes);
                    std::string node;
                    while (std::getline(ss, node, ',')) {
                        Vertex<T>* vertex = g->findVertex(std::stoi(node));
                        if (vertex) {
                            nodesToAvoid.push_back(vertex);
                        }
                    }
                } else if (line.rfind("AvoidSegments:", 0) == 0) {
                    std::string segments = line.substr(14);
                    std::stringstream ss(segments);
                    std::string segment;
                    while (std::getline(ss, segment, ')')) {
                        if (segment.front() == '(') segment = segment.substr(1);
                        if (segment.back() == ',') segment.pop_back();

                        std::string src = segment.substr(0, segment.find(','));
                        std::string dst = segment.substr(segment.find(',') + 1);
                        Vertex<T>* srcVertex = g->findVertex(std::stoi(src));
                        Vertex<T>* dstVertex = g->findVertex(std::stoi(dst));

                        if (srcVertex && dstVertex) {
                            Edge<T>* edge = g->findEdge(srcVertex->getLocation(), dstVertex->getLocation());
                            if (edge) {
                                segmentsToAvoid.push_back(edge);
                            }
                        }
                    }
                }
            }

            inFile.close();
        }
    else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        while (!validSource) {
            std::cout << "Please enter the source vertex (cannot have parking): ";
            std::getline(std::cin, source);
            Vertex<T>* sourceVertex = g->findVertex(source);
            if (sourceVertex != nullptr && !sourceVertex->getParking()) {
                validSource = true;
            } else {
                std::cout << "\nInvalid source vertex or vertex has parking. Please try again: " << std::endl;;
            }
        }

        while (!validTarget) {
            std::cout << "Please enter the target vertex (cannot be adjacent to the source or have parking): ";
            std::getline(std::cin, target);
            Vertex<T>* targetVertex = g->findVertex(target);
            if (targetVertex != nullptr && !targetVertex->getParking() && target != source) {
                Vertex<T>* sourceVertex = g->findVertex(source);
                bool isAdjacent = false;
                for (const auto& edge : sourceVertex->getAdj()) {
                    if (edge->getDest() == targetVertex) {
                        isAdjacent = true;
                        break;
                    }
                }
                if (!isAdjacent) {
                    validTarget = true;
                } else {
                    std::cout << "\nTarget vertex is adjacent to the source. Please try again: " << std::endl;
                }
            } else {
                std::cout << "\nInvalid target vertex, vertex has parking, or target is the same as the source. Please try again: " << std::endl;
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
                    std::cout << "\nInvalid node " << node << ". Please try again: " << std::endl;
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

        while (!validMaxWalkingTime) {
            std::cout << "Please enter the maximum walking time: ";
            std::string input;
            std::getline(std::cin, input);

            bool isValid = true;
            for (char c : input) {
                if (!isdigit(c)) {
                    isValid = false;
                    break;
                }
            }

            if (isValid && !input.empty()) {
                maxWalkingTime = std::stoi(input);
                validMaxWalkingTime = true;
            } else {
                std::cout << "\nInvalid input. Please enter a non-negative integer.\n";
            }
        }
    }


    Vertex<T>* sourceVertex = nullptr;
    Vertex<T>* targetVertex = nullptr;

    if (batch_mode) {
        sourceVertex = g->findVertex(sourceID);
        targetVertex = g->findVertex(targetID);
    } else {
        sourceVertex = g->findVertex(source);
        targetVertex = g->findVertex(target);
    }

    printFastestEnvFriendlyRoute(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid, maxWalkingTime);
}


template <class T>
void Menu<T>::alternativeEnvFriendlyRouts() {
    std::vector<Vertex<T>*> nodesToAvoid = {};
    std::vector<Edge<T>*> segmentsToAvoid = {};
    string source, target;
    int sourceID = -1, targetID = -1;
    int maxWalkingTime;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validMaxWalkingTime = false;

        if (batch_mode) {
            std::ifstream inFile("../input.txt");
            if (!inFile) {
                std::cerr << "Error opening input.txt" << std::endl;
                return;
            }

            std::string line;
            while (std::getline(inFile, line)) {
                if (line.rfind("Source:", 0) == 0) {
                    source = line.substr(7);
                    sourceID = std::stoi(source);
                } else if (line.rfind("Destination:", 0) == 0) {
                    target = line.substr(12);
                    targetID = std::stoi(target);
                } else if (line.rfind("MaxWalkTime:", 0) == 0) {
                    maxWalkingTime = std::stoi(line.substr(12));
                } else if (line.rfind("AvoidNodes:", 0) == 0) {
                    std::string nodes = line.substr(11);
                    std::stringstream ss(nodes);
                    std::string node;
                    while (std::getline(ss, node, ',')) {
                        Vertex<T>* vertex = g->findVertex(std::stoi(node));
                        if (vertex) {
                            nodesToAvoid.push_back(vertex);
                        }
                    }
                } else if (line.rfind("AvoidSegments:", 0) == 0) {
                    std::string segments = line.substr(14);
                    std::stringstream ss(segments);
                    std::string segment;
                    while (std::getline(ss, segment, ')')) {
                        if (segment.front() == '(') segment = segment.substr(1);
                        if (segment.back() == ',') segment.pop_back();

                        std::string src = segment.substr(0, segment.find(','));
                        std::string dst = segment.substr(segment.find(',') + 1);
                        Vertex<T>* srcVertex = g->findVertex(std::stoi(src));
                        Vertex<T>* dstVertex = g->findVertex(std::stoi(dst));

                        if (srcVertex && dstVertex) {
                            Edge<T>* edge = g->findEdge(srcVertex->getLocation(), dstVertex->getLocation());
                            if (edge) {
                                segmentsToAvoid.push_back(edge);
                            }
                        }
                    }
                }
            }

            inFile.close();
        }
    else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        while (!validSource) {
            std::cout << "Please enter the source vertex (cannot have parking): ";
            std::getline(std::cin, source);
            Vertex<T>* sourceVertex = g->findVertex(source);
            if (sourceVertex != nullptr && !sourceVertex->getParking()) {
                validSource = true;
            } else {
                std::cout << "\nInvalid source vertex or vertex has parking. Please try again: " << std::endl;;
            }
        }

        while (!validTarget) {
            std::cout << "Please enter the target vertex (cannot be adjacent to the source or have parking): ";
            std::getline(std::cin, target);
            Vertex<T>* targetVertex = g->findVertex(target);
            if (targetVertex != nullptr && !targetVertex->getParking() && target != source) {
                Vertex<T>* sourceVertex = g->findVertex(source);
                bool isAdjacent = false;
                for (const auto& edge : sourceVertex->getAdj()) {
                    if (edge->getDest() == targetVertex) {
                        isAdjacent = true;
                        break;
                    }
                }
                if (!isAdjacent) {
                    validTarget = true;
                } else {
                    std::cout << "\nTarget vertex is adjacent to the source. Please try again: " << std::endl;
                }
            } else {
                std::cout << "\nInvalid target vertex, vertex has parking, or target is the same as the source. Please try again: " << std::endl;
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
                    std::cout << "\nInvalid node " << node << ". Please try again: " << std::endl;
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

        while (!validMaxWalkingTime) {
            std::cout << "Please enter the maximum walking time: ";
            std::string input;
            std::getline(std::cin, input);

            bool isValid = true;
            for (char c : input) {
                if (!isdigit(c)) {
                    isValid = false;
                    break;
                }
            }

            if (isValid && !input.empty()) {
                maxWalkingTime = std::stoi(input);
                validMaxWalkingTime = true;
            } else {
                std::cout << "\nInvalid input. Please enter a non-negative integer.\n";
            }
        }
    }

    Vertex<T>* sourceVertex = nullptr;
    Vertex<T>* targetVertex = nullptr;

    if (batch_mode) {
        sourceVertex = g->findVertex(sourceID);
        targetVertex = g->findVertex(targetID);
    } else {
        sourceVertex = g->findVertex(source);
        targetVertex = g->findVertex(target);
    }

    printAlternativeEnvFriendlyRoutes(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid, maxWalkingTime);
}

template <class T>
void Menu<T>::printFastestIndependantRoute(Vertex<T>* source, Vertex<T>* target) {

    std::vector<Vertex<T>*> avoidNodes = {};

    dijkstra(g, source->getLocation(), {}, {}, true);

    std::pair<std::vector<string>, int> res = getPath(g, source->getLocation(), target->getLocation());

    for (int i = 1; i < res.first.size() - 1; i++) {
        avoidNodes.push_back(g->findVertex(res.first[i]));
    }

    dijkstra(g, source->getLocation(), avoidNodes, {}, true);

    std::pair<std::vector<std::string>, int> res2 = getPath(g, source->getLocation(), target->getLocation());

    if (batch_mode) {
        std::ofstream outFile("../output.txt");

        outFile << "Source:" << source->getLocation() << "\n";
        outFile << "Destination:" << target->getLocation() << "\n";

        outFile << "BestDrivingRoute:";
        for (size_t i = 0; i < res.first.size(); ++i) {
            outFile << res.first[i];
            if (i != res.first.size() - 1) outFile << ",";
        }
        outFile << "(" << res.second << ")\n";

        outFile << "AlternativeDrivingRoute:";
        for (size_t i = 0; i < res2.first.size(); ++i) {
            outFile << res2.first[i];
            if (i != res2.first.size() - 1) outFile << ",";
        }
        outFile << "(" << res2.second << ")\n";
        std::cout << "Successfully written result to output.txt" << std::endl;

        outFile.close();
    } else {
        std::cout << "Fastest Independent Route from " << source->getLocation() << " to " << target->getLocation() << std::endl;

        for (size_t i = 0; i < res.first.size(); ++i) {
            std::cout << res.first[i];
            if (i != res.first.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\nTotal distance: " << res.second << std::endl << std::endl;

        std::cout << "Fastest Alternative Independent Route from " << source->getLocation() << " to " << target->getLocation() << std::endl;

        for (size_t i = 0; i < res2.first.size(); ++i) {
            std::cout << res2.first[i];
            if (i != res2.first.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\nTotal distance: " << res2.second << std::endl;
    }
}

template <class T>
void Menu<T>::printFastestRestrictedRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
    std::vector<Edge<T>*> segmentsToAvoid) {

    dijkstra(g, source->getLocation(), nodesToAvoid, segmentsToAvoid, true);

    std::pair<std::vector<string>, int> res = getPath(g, source->getLocation(), target->getLocation());
    if (batch_mode) {
        std::ofstream outFile("../output.txt");
        if (!outFile) {
            std::cerr << "Error opening output.txt" << std::endl;
            return;
        }

        outFile << "Source:" << source->getLocation() << "\n";
        outFile << "Destination:" << target->getLocation() << "\n";
        outFile << "RestrictedDrivingRoute:";

        for (size_t i = 0; i < res.first.size(); ++i) {
            outFile << res.first[i];
            if (i != res.first.size() - 1) {
                outFile << ",";
            }
        }

        outFile << "(" << res.second << ")\n";

        std::cout << "Successfully written result to output.txt" << std::endl;
        outFile.close();

    }
    else {
        std::cout << "Fastest Restricted Route from " << source->getLocation() << " to " << target->getLocation() << std::endl;

        for (size_t i = 0; i < res.first.size(); ++i) {
            std::cout << res.first[i];
            if (i != res.first.size() - 1) {
                std::cout << " -> ";
            }
        }

        std::cout << "\nTotal distance: " << res.second << std::endl;
    }
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
    if (batch_mode) {
        std::ofstream outFile("../output.txt");
        if (!outFile) {
            std::cerr << "Error opening output.txt" << std::endl;
            return;
        }

        outFile << "Source:" << source->getLocation() << "\n";
        outFile << "Destination:" << target->getLocation() << "\n";
        outFile << "RestrictedDrivingRoute:";

        for (size_t i = 0; i < res.first.size(); ++i) {
            outFile << res.first[i];
            if (i != res.first.size() - 1) {
                outFile << ",";
            }
        }

        outFile << "(" << totaldist << ")\n";

        std::cout << "Successfully written result to output.txt" << std::endl;
        outFile.close();

    } else {
        std::cout << "Fastest Restricted Route from " << source->getLocation() << " to " << includeNode->getLocation() << std::endl;

        for (size_t i = 0; i < res.first.size(); ++i) {
            std::cout << res.first[i];
            if (i != res.first.size() - 1) {
                std::cout << " -> ";
            }
        }
    }
    std::cout << "\nTotal distance: " << totaldist << std::endl;
}

template <class T>
void Menu<T>::printFastestEnvFriendlyRoute(Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
                                 std::vector<Edge<T>*> segmentsToAvoid, int maxWalkingTime) {
    std::string origin = source->getLocation();
    std::string destination = target->getLocation();

    dijkstra(g, origin, nodesToAvoid, segmentsToAvoid, true); // Running dijkstra to get the shortest paths to each parking node

    std::vector<std::tuple<std::vector<std::string>, int, std::string>> driving_paths;
    for (Vertex<T> *v : g->getVertexSet()) { // Building the shortest driving paths (the ones who end in a node with parking)
        if (v->getParking() && v != g->findVertex(origin)) {
            std::pair<std::vector<std::string>, int> path = getPath(g, origin, v->getLocation());
            driving_paths.push_back(std::make_tuple(path.first, path.second, v->getLocation()));
        }
    }

    std::vector<std::pair<std::vector<std::string>, int>> walking_paths;
    for (const auto& driving_path: driving_paths) { // Building the shortest walking paths starting in a parking node
        const std::vector<std::string>& pathNodes = std::get<0>(driving_path);
        std::string parkingNode = pathNodes.back();
        dijkstra(g, parkingNode, nodesToAvoid, segmentsToAvoid, false); // Running dijkstra to get the shortest paths form each parking node to the destination

        std::pair<std::vector<std::string>, int> path = getPath(g, parkingNode, destination);
        walking_paths.push_back(path);
    }

    std::vector<std::tuple<std::vector<std::string>, int, std::string>> possible_paths;
    std::vector<int> walking_times;
    for (const auto& driving_path: driving_paths) { // Combining the times of the valid driving + walking paths
        const std::vector<std::string>& drivingPathNodes = std::get<0>(driving_path);
        std::string parkingNode = drivingPathNodes.back();
        const int drivingTime = std::get<1>(driving_path);

        for (const auto& walking_path: walking_paths) {
            const std::vector<std::string>& walkingPathNodes = walking_path.first;
            std::string startingWalkingNode = walkingPathNodes.front();
            const int walkingTime = walking_path.second;

            if (startingWalkingNode == parkingNode && walkingTime <= maxWalkingTime && walkingTime > 0) { // Not accepting paths with no walking
                std::vector<std::string> fullPath = drivingPathNodes;  // Start with the driving path
                fullPath.insert(fullPath.end(), walkingPathNodes.begin()+1, walkingPathNodes.end());

                int fullPathTime = drivingTime + walkingTime;
                possible_paths.emplace_back(fullPath, fullPathTime, parkingNode);
                walking_times.push_back(walkingTime);
            }
        }
    }

    std::tuple<std::vector<std::string>, int, std::string> shortestPath;
    int minTime = 999;
    int maxWalkingTimeInTie = -1;
    for (size_t i = 0; i < possible_paths.size(); ++i) { // Finding the shortest total path
        const auto& path = possible_paths[i];
        int currentTime = std::get<1>(path);
        int currentWalkingTime = walking_times[i];
        if (currentTime < minTime || (currentTime == minTime && currentWalkingTime > maxWalkingTimeInTie)) {
            shortestPath = path;
            minTime = currentTime;
            maxWalkingTimeInTie = currentWalkingTime;
        }
    }

    if (!possible_paths.empty()) {
        if (batch_mode) {
            std::ofstream outputFile("../output.txt");

            outputFile << "Source:" << origin << "\n";
            outputFile << "Destination:" << destination << "\n";

            // Driving Route
            outputFile << "DrivingRoute:";
            for (const auto& node : std::get<0>(shortestPath)) {
                if (node != std::get<2>(shortestPath)) {
                    outputFile << node << ",";
                } else {
                    break;
                }
            }
            outputFile.seekp(-1, std::ios_base::cur); // Remove last comma
            outputFile << "(" << std::get<1>(shortestPath) << ")\n";

            // Parking Node
            outputFile << "ParkingNode:" << std::get<2>(shortestPath) << "\n";

            // Walking Route
            outputFile << "WalkingRoute:";
            bool walkingPhase = false;
            for (const auto& node : std::get<0>(shortestPath)) {
                if (node == std::get<2>(shortestPath)) walkingPhase = true;
                if (walkingPhase) {
                    outputFile << node << ",";
                }
            }
            outputFile.seekp(-1, std::ios_base::cur);
            outputFile << "(" << std::get<1>(shortestPath) << ")\n";

            outputFile << "TotalTime:" << std::get<1>(shortestPath) << "\n";

            outputFile.close();
        }
        else {
            std::cout << "Driving Route: ";
            for (const auto& node : std::get<0>(shortestPath)) {
                if (node != std::get<2>(shortestPath)) {
                    std::cout << node << " -> ";
                }
                else break;
            }
            std::cout << std::get<2>(shortestPath) << std::endl;

            std::cout << "Parking Node: " << std::get<2>(shortestPath) << "\n";

            std::cout << "Walking Route: ";
            bool walkingPhase = false;
            for (const auto& node : std::get<0>(shortestPath)) {
                if (node == std::get<2>(shortestPath)) walkingPhase = true;
                if (walkingPhase) {
                    std::cout << node;
                    if (node != destination) std::cout << " -> ";
                }
            }
            std::cout << "\n";

            std::cout << "Total Time: " << std::get<1>(shortestPath) << " minutes\n";
        }
    }
    else {
        std::string message = "No valid route found within the given constraints.";
        if (driving_paths.empty())
            message += " No available parking within reach.";

        if (walking_paths.empty())
            message += " No walking path found to the destination.";

        else message += " All walking times exceed the maximum allowed.";

        if (batch_mode) {
            std::ofstream outputFile("../output.txt");
            outputFile << "Source:" << origin << "\n";
            outputFile << "Destination:" << destination << "\n";
            outputFile << "DrivingRoute:none\n";
            outputFile << "ParkingNode:none\n";
            outputFile << "WalkingRoute:none\n";
            outputFile << "TotalTime:\n";
            outputFile << "Message:" << message << "\n";

            std::cout << "Successfully written result to output.txt" << std::endl;
            outputFile.close();
        }
        else std::cout << message << std::endl;
    }
}

template <class T>
void Menu<T>::printAlternativeEnvFriendlyRoutes(Vertex<T>* source, Vertex<T>* target, vector<Vertex<T>*> nodesToAvoid, vector<Edge<T>*> segmentsToAvoid, int maxWalkingTime) {
    std::string origin = source->getLocation();
    std::string destination = target->getLocation();

    dijkstra(g, origin, nodesToAvoid, segmentsToAvoid, true);

    std::vector<std::tuple<std::vector<std::string>, int, std::string>> driving_paths;
    for (Vertex<T> *v : g->getVertexSet()) {
        if (v->getParking() && v != g->findVertex(origin)) {
            auto path = getPath(g, origin, v->getLocation());
            driving_paths.emplace_back(path.first, path.second, v->getLocation());
        }
    }

    std::vector<std::pair<std::vector<std::string>, int>> walking_paths;
    for (const auto& driving_path : driving_paths) {
        std::string parkingNode = std::get<0>(driving_path).back();
        dijkstra(g, parkingNode, nodesToAvoid, segmentsToAvoid, false);

        auto path = getPath(g, parkingNode, destination);
        walking_paths.push_back(path);
    }

    std::vector<std::tuple<std::vector<std::string>, int, std::string>> alternativePaths;
    std::vector<int> walking_times;
    for (size_t i = 0; i < driving_paths.size(); ++i) {
        int walkingTime = walking_paths[i].second;
        if (walkingTime > maxWalkingTime) {
            std::vector<std::string> fullPath = std::get<0>(driving_paths[i]);
            fullPath.insert(fullPath.end(), walking_paths[i].first.begin() + 1, walking_paths[i].first.end());

            int totalTime = std::get<1>(driving_paths[i]) + walkingTime;
            alternativePaths.emplace_back(fullPath, totalTime, std::get<2>(driving_paths[i]));
            walking_times.push_back(walkingTime);
        }
    }

    std::sort(alternativePaths.begin(), alternativePaths.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    if (alternativePaths.size() >= 2) {
        if (batch_mode) {
            std::ofstream outputFile("../output.txt");

            outputFile << "Source:" << origin << "\n";
            outputFile << "Destination:" << destination << "\n";

            for (size_t i = 0; i < 2; i++) {
                auto bestPath = alternativePaths[i];

                // Driving Route
                outputFile << "DrivingRoute" << i + 1 << ":";
                for (const auto& node : std::get<0>(bestPath)) {
                    if (node != std::get<2>(bestPath)) {
                        outputFile << node << ",";
                    } else break;
                }
                outputFile.seekp(-1, std::ios_base::cur);
                outputFile << "(" << std::get<1>(bestPath) << ")\n";

                // Parking Node
                outputFile << "ParkingNode" << i + 1 << ":" << std::get<2>(bestPath) << "\n";

                // Walking Route
                outputFile << "WalkingRoute" << i + 1 << ":";
                bool walkingPhase = false;
                for (const auto& node : std::get<0>(bestPath)) {
                    if (node == std::get<2>(bestPath)) walkingPhase = true;
                    if (walkingPhase) {
                        outputFile << node << ",";
                    }
                }
                outputFile.seekp(-1, std::ios_base::cur);
                outputFile << "(" << walking_times[i] << ")\n";

                outputFile << "TotalTime" << i + 1 << ":" << std::get<1>(bestPath) << "\n";
            }

            std::cout << "Successfully written result to output.txt" << std::endl;
            outputFile.close();
        }

        else {
            for (int i = 0; i < 2; ++i) {
                auto bestPath = alternativePaths[i];
                std::cout << "Alternative "<< i+1 << ":" << std::endl;
                std::cout << "Source:" << origin << "\n";
                std::cout << "Destination:" << destination << "\n";
                std::cout << "DrivingRoute" << i + 1 << ":";
                for (const auto& node : std::get<0>(bestPath)) {
                    if (node != std::get<2>(bestPath)) std::cout << node << ",";
                    else break;
                }
                std::cout << "(" << std::get<1>(bestPath) << ")\n";

                std::cout << "ParkingNode" << i + 1 << ":" << std::get<2>(bestPath) << "\n";

                std::cout << "WalkingRoute" << i + 1 << ":";
                bool walkingPhase = false;
                for (const auto& node : std::get<0>(bestPath)) {
                    if (node == std::get<2>(bestPath)) walkingPhase = true;
                    if (walkingPhase) std::cout << node << ",";
                }
                std::cout << "(" << walking_times[i] << ")\n";

                std::cout << "TotalTime" << i + 1 << ":" << std::get<1>(bestPath) << "\n";
            }
        }

    } else {
        std::cout << "No alternative routes found.\n";
    }
}

template <class T>
void Menu<T>::printInitialMenu() {
    unsigned int option;
    bool exit = false;

    std::cout << "1. Find the fastest independent route\n";
    std::cout << "2. Find the fastest restricted route\n";
    std::cout << "3. Find the fastest environmentally friendly route\n";
    std::cout << "4. Find the alternatives for environmentally friendly route\n";
    std::cout << "5. Exit\n";

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
                this->fastestEnvFriendlyRoute();
                exit = true;
                break;
            case 4:
                this->alternativeEnvFriendlyRouts();
                exit = true;
                break;
            case 5:
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
