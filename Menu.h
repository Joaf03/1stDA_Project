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
                                     std::vector<Edge<T>*> segmentsToAvoid, int maxWalkingTime);

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
void Menu<T>::fastestEnvFriendlyRoute() {
    std::vector<Vertex<T>*> nodesToAvoid = {};
    std::vector<Edge<T>*> segmentsToAvoid = {};
    string source, target;
    int maxWalkingTime;
    bool validSource = false, validTarget = false, validAvoidNodes = false, validAvoidSegments = false, validMaxWalkingTime = false;

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


    Vertex<T>* sourceVertex = g->findVertex(source);
    Vertex<T>* targetVertex = g->findVertex(target);

    printFastestEnvFriendlyRoute(sourceVertex, targetVertex, nodesToAvoid, segmentsToAvoid, maxWalkingTime);
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
    } else {
        std::cout << "No valid route found within the given constraints.\n";
    }
}

template <class T>
void Menu<T>::printInitialMenu() {
    unsigned int option;
    bool exit = false;

    std::cout << "1. Find the fastest independent route\n";
    std::cout << "2. Find the fastest restricted route\n";
    std::cout << "3. Find the fastest environmentally friendly route\n";
    std::cout << "4. Exit\n";

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
