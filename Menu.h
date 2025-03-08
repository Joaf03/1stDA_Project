//
// Created by curlmike on 08-03-2025.
//

#ifndef MENU_H
#define MENU_H
#include <Graph.h>

template <class T>
class Menu {
public:
    Menu(Graph<T>* g): g(g) {}

    void printInitialMenu();

    void fastestIndependantRoute(Graph<T> *g);

    void fastestRestrictedRoute(Graph<T> *g);

    void printFastestIndependantRoute(Graph<T> *g, Vertex<T>* source, Vertex<T>* target);

    void printFastestRestrictedRoute(Graph<T> *g, Vertex<T>* source, Vertex<T>* target, std::vector<Vertex<T>*> nodesToAvoid,
        std::vector<Edge<T>*> segmentsToAvoid, Vertex<T>* includeNode);

    Graph<T>* getGraph() const {
        return g;
    }

protected:
    Graph<T>* g;
};

#endif //MENU_H
