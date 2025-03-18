//
// Created by curlmike on 18-03-2025.
//

#ifndef INC_1STDA_PROJECT_GRAPH_H
#define INC_1STDA_PROJECT_GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include "../data_structures/MutablePriorityQueue.h"

using namespace std;

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

// Hash function for std::pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    Vertex(T in);

    Vertex(int ID, string Code, string Location, bool hasParking)
            : ID(ID), Code(Code), Location(Location), hasParking(hasParking) {}

    int getID() const { return ID; }

    string getCode() const { return Code; }

    string getLocation() const { return Location; }

    bool getParking() const { return hasParking; }

    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    void setInfo(T info);
    void setVisited(bool visited);
    void setProcessing(bool processing);

    int getLow() const;
    void setLow(int value);
    int getNum() const;
    void setNum(int value);

    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);
    Edge<T> * addEdge(Vertex<T> *dest, double w);
    bool removeEdge(T in);
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    T info;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    int low = -1, num = -1; // used by SCC Tarjan
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int ID;
    string Code;
    string Location;
    bool hasParking;

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double w);
    Edge(Vertex<T>* orig, Vertex<T>* dest, int drivingTime, int walkingTime)
            : orig(orig), dest(dest), drivingTime(drivingTime), walkingTime(walkingTime) {}


    Vertex<T> * getDest() const;
    double getWeight() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);
    void setFlow(double flow);

    int getDrivingTime() const { return drivingTime; }

    int getWalkingTime() const { return walkingTime; }

protected:
    Vertex<T> * dest; // destination vertex
    double weight; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    int drivingTime;
    int walkingTime;

    double flow; // for flow-related problems
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:

    Graph() = default;

    ~Graph();
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T> *findVertex(int ID) const;

    Vertex<T> *findVertex(string location) const;

    Edge<T> *findEdge(string orig, string dest) const;

    bool addVertex(int ID, string Code, string Location, bool hasParking);

    bool addEdge(string orig, string dest, int drivingTime, int walkingTime);

    bool removeVertex(int ID);

    bool removeEdge(string orig, string dest);

    vector<Vertex<T> *> getVertices() const;

    vector<Edge<T> *> getEdges() const;

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, double w);
    bool removeEdge(const T &source, const T &dest);
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    int getNumVertex() const;

    std::vector<Vertex<T> *> getVertexSet() const;


protected:
    std::vector<Vertex<T> *> vertexSet;
    unordered_map<int, Vertex<T> *> vertexMap; // used for a more efficient search
    unordered_map<pair<int, int>, Edge<T> *, pair_hash> edgeMap;
    vector<Edge<T> *> edges;

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const T &in) const;
    /**
    * Auxiliary function to set the "path" field to make a spanning tree.
    */

};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}
/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template <class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template <class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template <class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w): orig(orig), dest(dest), weight(w) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

template <class T>
Vertex<T> *Graph<T>::findVertex(int ID) const {
    auto it = vertexMap.find(ID);
    return it != vertexMap.end() ? it->second : nullptr;
}

template <class T>
Vertex<T> *Graph<T>::findVertex(string location) const {
    auto it = vertexMap.find(location);
    return it != vertexMap.end() ? it->second : nullptr;
}

template <class T>
Edge<T> *Graph<T>::findEdge(string orig, string dest) const {
    auto it = edgeMap.find({orig, dest});
    if (it == edgeMap.end()) it = edgeMap.find({dest, orig});
    return it != edgeMap.end() ? it->second : nullptr;
}

template <class T>
bool Graph<T>::addVertex(int ID, string Code, string Location, bool hasParking) {
    if (vertexMap.find(ID) != vertexMap.end()) return false;
    Vertex<T> *v = new Vertex<T>(ID, Code, Location, hasParking);
    vertexSet.push_back(v);
    vertexMap[ID] = v;
    return true;
}

template <class T>
bool Graph<T>::addEdge(string orig, string dest, int drivingTime, int walkingTime) {
    if (edgeMap.find({orig, dest}) != edgeMap.end()) return false;
    if (edgeMap.find({dest, orig}) != edgeMap.end()) return false;
    Edge<T> *e = new Edge<T>(orig, dest, drivingTime, walkingTime); // original edge
    Edge<T> *e2 = new Edge<T>(dest, orig, drivingTime, walkingTime); // reverse edge
    edges.push_back(e);
    edges.push_back(e2);
    edgeMap[{orig, dest}] = e;
    edgeMap[{orig, dest}] = e2;
    return true;
}

template <class T>
bool Graph<T>::removeVertex(int ID) {
    Vertex<T> *v = findVertex(ID);
    if (!v) return false;

    // Remove all edges connected to this vertex
    auto it = edges.begin();
    while (it != edges.end()) {
        if ((*it)->getOrig() == v || (*it)->getDest() == v) {
            edgeMap.erase({(*it)->getOrig()->getID(), (*it)->getDest()->getID()});
            delete *it;
            it = edges.erase(it);
        }
        else ++it;
    }

    // Remove the vertex
    vertexMap.erase(ID);
    vertexSet.erase(remove(vertexSet.begin(), vertexSet.end(), v), vertexSet.end());
    delete v;

    return true;
}

template <class T>
bool Graph<T>::removeEdge(string orig, string dest) {
    Edge<T> *e = findEdge(orig, dest);
    Edge<T> *e2 = findEdge(dest, orig);
    if (!(e || e2)) return false;

    // Remove the edge
    auto it = edges.begin();
    while (it != edges.end()) {
        if ((*it)->getOrig() == orig && (*it)->getDest() == dest) {
            edgeMap.erase({(*it)->getOrig()->getID(), (*it)->getDest()->getID()});
            delete *it;
            it = edges.erase(it);
        }
        else if ((*it)->getOrig() == dest && (*it)->getDest() == orig) {
            edgeMap.erase({(*it)->getDest()->getID(), (*it)->getOrig()->getID()});
            delete *it;
            it = edges.erase(it);
        }
        else ++it;
    }

    delete e;;
    delete e2;

    return true;
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertices() const {
    return vertexSet;
}

template <class T>
vector<Edge<T> *> Graph<T>::getEdges() const {
    return edges;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}


#endif //GRAPH1_H
