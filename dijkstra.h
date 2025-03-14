//
// Created by curlmike on 14-03-2025.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <Graph.h>
#include <queue>
#include <limits>
#include <algorithm>

template <class T>
bool relaxEdge(Edge<T> *edge);

template <class T>
void dijkstra(Graph<T>* g, const string &origin, std::vector<Vertex<T>*> nodesToAvoid, std::vector<Edge<T>*> segmentsToAvoid);

template <class T>
std::vector<T> getPath(Graph<T> * g, const string &origin, const string &dest);

#endif //DIJKSTRA_H
