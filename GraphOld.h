//
// Created by Outros on 05/03/2025.
//

#ifndef INC_1STDA_PROJECT_GRAPH_OLD_H
#define INC_1STDA_PROJECT_GRAPH_OLD_H

#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

// Hash function for std::pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

//------------------------------VERTEX------------------------------//

template <class T>
class Vertex {
public:
    Vertex(int ID, string Code, string Location, bool hasParking)
            : ID(ID), Code(Code), Location(Location), hasParking(hasParking) {}

    int getID() const { return ID; }

    string getCode() const { return Code; }

    string getLocation() const { return Location; }

    bool getParking() const { return hasParking; }

    int getDist() const { return dist; }

    void setDist(int dist) { this->dist = dist; }

protected:
    int ID;
    int dist;
    string Code;
    string Location;
    bool hasParking;
};

//------------------------------EDGE------------------------------//

template <class T>
class Edge {
public:
    Edge(Vertex<T>* orig, Vertex<T>* dest, int drivingTime, int walkingTime)
            : orig(orig), dest(dest), drivingTime(drivingTime), walkingTime(walkingTime) {}

    Vertex<T> * getOrig() const { return orig; }

    Vertex<T> * getDest() const { return dest; }

    int getDrivingTime() const { return drivingTime; }

    int getWalkingTime() const { return walkingTime; }

protected:
    int ID;
    Vertex<T> * orig;
    Vertex<T> * dest;
    int drivingTime;
    int walkingTime;
};

//------------------------------GRAPH------------------------------//

template <class T>
class Graph {
public:
    Graph() = default;

    ~Graph() {
        for (auto vertex : vertices) delete vertex;
        for (auto edge : edges) delete edge;
    }

    Vertex<T> *findVertex(int ID) const {
        auto it = vertexMap.find(ID);
        return it != vertexMap.end() ? it->second : nullptr;
    }

    Vertex<T> *findVertex(string location) const {
        auto it = vertexMap.find(location);
        return it != vertexMap.end() ? it->second : nullptr;
    }

    Edge<T> * findEdge(string orig, string dest) const {
        auto it = edgeMap.find({orig, dest});
        if (it == edgeMap.end()) it = edgeMap.find({dest, orig});
        return it != edgeMap.end() ? it->second : nullptr;
    }

    bool addVertex(int ID, string Code, string Location, bool hasParking) {
        if (vertexMap.find(ID) != vertexMap.end()) return false;
        Vertex<T> *v = new Vertex<T>(ID, Code, Location, hasParking);
        vertices.push_back(v);
        vertexMap[ID] = v;
        return true;
    }

    bool addEdge(string orig, string dest, int drivingTime, int walkingTime) {
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

    bool removeVertex(int ID) {
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
        vertices.erase(remove(vertices.begin(), vertices.end(), v), vertices.end());
        delete v;

        return true;
    }

    bool removeEdge(string orig, string dest) {
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

        vector<Vertex<T> *> getVertices() const {
            return vertices;
        }

        vector<Edge<T> *> getEdges() const {
            return edges;
        }

    void parseLocations(const string &filename) {
        ifstream file(filename);
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string location, code;
            int id, parking;
            getline(ss, location, ',');
            ss >> id;
            ss.ignore();
            getline(ss, code, ',');
            ss >> parking;
            addVertex(id, code, location, parking);
            addVertex(id, code, location, parking);
        }
    }

    void parseDistances(const string &filename) {
        ifstream file(filename);
        string line;
        getline(file, line);
        while (getline(file, line)) {
            stringstream ss(line);
            string loc1, loc2, driving, walking;
            getline(ss, loc1, ',');
            getline(ss, loc2, ',');
            getline(ss, driving, ',');
            getline(ss, walking, ',');
            int drivingTime = (driving == "X") ? numeric_limits<int>::infinity() : stoi(driving);
            int walkingTime = stoi(walking);
            addEdge(loc1, loc2, drivingTime, walkingTime);
        }
    }

private:
    unordered_map<int, Vertex<T> *> vertexMap; // used for a more efficient search
    unordered_map<pair<int, int>, Edge<T> *, pair_hash> edgeMap;
    vector<Vertex<T> *> vertices;
    vector<Edge<T> *> edges;
};

#endif //INC_1STDA_PROJECT_GRAPH_H
