#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <climits>

using namespace std;
typedef unsigned int uInt;

struct Edge {
    uInt start;
    uInt destination;
    uInt weight;
    Edge(const uInt start, const uInt destination, const uInt weight) :start(start), destination(destination), weight(weight) {};
};

struct Node {
    uInt index = 0;
    uInt dst = UINT_MAX / 2;
    uInt weight = UINT_MAX;
    bool visited = false;
    Node* next = nullptr;
    Node* predecessor = nullptr; // only used for searching shortest path
    void addEdge(Node* destination, uInt w) {
        Node* currNode = this;
        while (currNode->next) {
            currNode = currNode->next;
        }
        currNode->next = new Node();
        currNode->next->index = destination->index;
        currNode->next->weight = w;
    }
    ~Node() {
        if (!this->next) return;
        delete this->next;
    }
};

typedef pair<uInt, Node*> DNPair;

struct comparePair {
    bool operator() (const DNPair& d1, const DNPair& d2) { return d1.first > d2.first; }
};

class Graph {
private:
    uInt size;
    uInt start;
    uInt destination;
    vector<Node*> adjList;
public:
    explicit Graph(uInt size, uInt start, uInt destination, vector<Edge> v) {
        this->size = size;
        this->start = start;
        this->destination = destination;
        for (uInt i = 0; i<size; i++) {
            Node* currNode = new Node();
            currNode->index = i;
            this->adjList.push_back(currNode);
        }
        for (auto& edge : v) {
            adjList[edge.start]->addEdge(adjList[edge.destination], edge.weight);
        }
        cout << "INIT DONE" << endl;
    };

    ~Graph() {
        for (uInt i = 0; i<size; i++) {
            delete this->adjList[i];
        }
    }

    void findShortestPath() {
        priority_queue<DNPair, vector<DNPair>, comparePair> q;
        q.push(make_pair(0, this->adjList[start]));
        while (!q.empty()) {
            DNPair top = q.top();
            q.pop();
            cout << "POP" << endl;
            if (top.second->index != top.first) {
                continue;
            } // handle duplicates
            Node* currNode = top.second->next;
            while (currNode) {
                if (!adjList[currNode->index]->visited && adjList[currNode->index]->dst > top.second->dst + currNode->weight) {
                    adjList[currNode->index]->dst = top.second->dst + currNode->weight;
                    q.push(make_pair(adjList[currNode->index]->dst, adjList[currNode->index]));
                }
                currNode = currNode->next;
            }
            top.second->visited = true;
        }
        cout << adjList[this->destination]->dst;
    };
};


int main() {
    uInt size, start, destination;
    cin >> size >> start >> destination;
    string str;
    istringstream iStream;
    vector<Edge> e;
    uInt edgeStart, edgeDest, edgeWeight;
    while (getline(cin, str)) {
        iStream.str(str);
        iStream >> edgeStart >> edgeDest >> edgeWeight;
        e.push_back(Edge(edgeStart, edgeDest, edgeWeight));
    }
    Graph g = Graph(size, start, destination, e);
    g.findShortestPath();
}