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
bool relaxEdge(Edge<T> *edge) { // d[u] + w(u,v) < d[v]

    Vertex<T>* origin = edge->getOrig();
    Vertex<T>* dest = edge->getDest();

    if (origin->getDist() + edge->getWeight() < dest->getDist()) {
        dest->setDist(origin->getDist() + edge->getWeight());
        dest->setPath(edge);
        return true;
    }

    return false;
}

template <class T>
void dijkstra(Graph<T> *g, const string &origin, std::vector<Vertex<T>*> nodesToAvoid, std::vector<Edge<T>*> segmentsToAvoid) {
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
            if (relaxEdge(e)) {
                if (!u->isVisited() && std::find(nodesToAvoid.begin(), nodesToAvoid.end(), u) == nodesToAvoid.end()) {
                    q.push(u);
                }
            }
        }
    }
}

template <class T>
std::vector<string> getPath(Graph<T> * g, const string &origin, const string &dest) {
    std::vector<std::string> res;

    Vertex<T>* dest_vertex = g->findVertex(dest);

    while (dest_vertex != nullptr && dest_vertex->getLocation() != origin) {
        res.push_back(dest_vertex->getLocation());
        dest_vertex = dest_vertex->getPath() ? dest_vertex->getPath()->getOrig() : nullptr;
    }

    if (dest_vertex != nullptr) {
        res.push_back(dest_vertex->getLocation());
    }

    std::reverse(res.begin(), res.end());

    return res;
}

#endif //DIJKSTRA_H
