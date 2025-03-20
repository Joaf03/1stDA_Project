//
// Created by curlmike on 14-03-2025.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "./data_structures/Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

template <class T>
bool relaxEdge(Edge<T> *edge, bool isDriving) { // d[u] + w(u,v) < d[v]

    Vertex<T>* origin = edge->getOrig();
    Vertex<T>* dest = edge->getDest();

    // If we are only using driving time, we relax the edges using only that time
    // Otherwise, we use only the walking time
    if (isDriving) {
        if (origin->getDist() + edge->getDrivingTime() < dest->getDist()) {
            dest->setDist(origin->getDist() + edge->getDrivingTime());
            dest->setPath(edge);
            return true;
        }
    } else {
        if (origin->getDist() + edge->getWalkingTime() < dest->getDist()) {
            dest->setDist(origin->getDist() + edge->getWalkingTime());
            dest->setPath(edge);
            return true;
        }
    }

    return false;
}

template <class T>
void dijkstra(Graph<T> *g, const string &origin, std::vector<Vertex<T>*> nodesToAvoid, std::vector<Edge<T>*> segmentsToAvoid, bool isDriving) {
    auto compare = [](Vertex<T>* left, Vertex<T>* right) { return left->getDist() > right->getDist(); };
    std::priority_queue<Vertex<T>*, std::vector<Vertex<T>*>, decltype(compare)> q(compare);

    for (Vertex<T>* v: g->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
        v->setDist(std::numeric_limits<double>::infinity());
    }

    Vertex<T>* start = g->findVertex(origin);
    q.push(start);
    start->setDist(0);

    while (!q.empty()) {
        Vertex<T>* v = q.top();
        q.pop();
        v->setVisited(true);

        for (Edge<T>* e: v->getAdj()) {
            if (std::find(segmentsToAvoid.begin(), segmentsToAvoid.end(), e) != segmentsToAvoid.end()) continue;
            Vertex<T>* u = e->getDest();
            if (relaxEdge(e, isDriving)) {
                if (!u->isVisited() && std::find(nodesToAvoid.begin(), nodesToAvoid.end(), u) == nodesToAvoid.end()) {
                    q.push(u);
                }
            }
        }
    }
}

template <class T>
std::pair<std::vector<string>, int> getPath(Graph<T> * g, const string &origin, const string &dest) {
    std::pair<std::vector<string>, int> res;
    std::vector<std::string> path;
    int dist = 0;

    Vertex<T>* dest_vertex = g->findVertex(dest);
    dist = dest_vertex->getDist();

    while (dest_vertex != nullptr && dest_vertex->getLocation() != origin) {
        path.push_back(dest_vertex->getLocation());
        dest_vertex = dest_vertex->getPath() ? dest_vertex->getPath()->getOrig() : nullptr;
    }

    if (dest_vertex != nullptr) {
        path.push_back(dest_vertex->getLocation());
    }

    std::reverse(path.begin(), path.end());

    res.first = path;
    res.second = dist;

    return res;
}

std::pair<std::vector<string>, int>> getEnvFriendlyPath(Graph<T> * g, const string &origin, std::vector<Vertex<T>*> nodesToAvoid, std::vector<Edge<T>*> segmentsToAvoid, cons string &destination, int maxWalkingTime) {
    dijkstra(g, origin, nodesToAvoid, segmentsToAvoid, true); // Running dijkstra to get the paths to each parking node

    std::vector<std::pair<std::vector<string>, int>> driving_paths;
    for (Vertex<T> *v : g->getVertexSet()) {
        if (v->getParking() && v != g->findVertex(origin)) {
            std::pair<std::vector<string>, int> path = getPath(origin, v);
            driving_paths.push_back(path);
        }
    }

    std::vector<std::pair<std::vector<string>, int>> walking_paths;
    for (std::pair<std::vector<string>, int> driving_path: driving_paths) {
        const std::vector<string>& pathNodes = driving_path.first;
        string parkingNode = pathNodes.back();
        dijkstra(g, parkingNode, nodesToAvoid, segmentsToAvoid, false) // Running dijkstra to get the paths form the parking node to the destination

        std::pair<std::vector<string>, int> path = getPath(parkingNode, destination);
        walking_paths.push_back(path);
    }

    std::vector<std::pair<std::vector<string>, int> possible_paths;
    // Combining the distances of the valid driving + walking paths
    for (std::pair<std::vector<string>, int> driving_path: driving_paths) {
        const std::vector<string>& drivingPathNodes = driving_path.first;
        string parkingNode = drivingPathNodes.back();
        const int drivingDistance = driving_path.second

        for (std::pair<std::vector<string>, int> walking_path: walking_paths) {
            const std::vector<string>& walkingPathNodes = walking_path.first;
            string startingWalkingNode = walkingPathNodes.front();
            const int walkingDistance = walking_path.second;

            if (startingWalkingNode == parkingNode && walkingDistance <= maxWalkingDistance) {
                std::vector<string> fullPath = drivingPathNodes;  // Start with the driving path
                fullPath.insert(fullPath.end(), walkingPathNodes.begin(), walkingPathNodes.end());

                int fullPathDistance = drivingDistance + walkingDistance;
                std::pair<std::vector<string>, int> full_path = {fullPath, fullPathDistance};
                possible_paths.push_back(full_path);
            }
        }
    }

    std::pair<std::vector<string>, int> shortestPath;
    for (std::pair<std::vector<string>, int> path: possible_paths) {
        std::vector<string> currentPath = path.first;
        int currentTime = path.second;
        if (currentMinTime < mintime) shortestPath = path;
    }

    return shortestPath;
}

#endif //DIJKSTRA_H
