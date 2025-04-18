//
// Created by curlmike on 14-03-2025.
//

/**
 * @file dijkstra.h
 * @brief Header file for Dijkstra's algorithm implementation.
 *
 * This file contains the implementation of Dijkstra's algorithm for finding the shortest paths in a graph.
 */

#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "./data_structures/Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

/**
 * @brief Relaxes an edge during the Dijkstra's algorithm.
 *
 * @tparam T The type of the vertex information.
 * @param edge Pointer to the edge to be relaxed.
 * @param isDriving Flag to indicate if the driving time should be used.
 * @return True if the edge was relaxed, false otherwise.
 *
 * @complexity O(1)
 */

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

/**
 * @brief Runs Dijkstra's algorithm on the graph.
 *
 * @tparam T The type of the vertex information.
 * @param g Pointer to the graph object.
 * @param origin The starting vertex location.
 * @param nodesToAvoid Vector of nodes to avoid.
 * @param segmentsToAvoid Vector of segments to avoid.
 * @param isDriving Flag to indicate if the driving time should be used.
 *
 * @complexity O((V + E) \log V), where V is the number of vertices and E is the number of edges.
 */

template <class T>
void dijkstra(Graph<T> *g, const string &origin, std::vector<Vertex<T>*> nodesToAvoid, std::vector<Edge<T>*> segmentsToAvoid, bool isDriving) {
    auto compare = [](Vertex<T>* left, Vertex<T>* right) { return left->getDist() > right->getDist(); };
    std::priority_queue<Vertex<T>*, std::vector<Vertex<T>*>, decltype(compare)> q(compare);

    for (Vertex<T>* v: g->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
        v->setDist(500000);
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
            if (std::find(nodesToAvoid.begin(), nodesToAvoid.end(), u) == nodesToAvoid.end()) {
                if (relaxEdge(e, isDriving)) {
                    if (!u->isVisited()) {
                        q.push(u);
                    }
                }
            }
        }
    }
}


/**
 * @brief Retrieves the path from the origin to the destination.
 *
 * @tparam T The type of the vertex information.
 * @param g Pointer to the graph object.
 * @param origin The starting vertex location.
 * @param dest The destination vertex location.
 * @return A pair containing the path as a vector of strings and the total distance.
 *
 * @complexity O(V), where V is the number of vertices in the path.
 */
template <class T>
std::pair<std::vector<std::string>, int> getPath(Graph<T> * g, const string &origin, const string &dest) {
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

#endif //DIJKSTRA_H
