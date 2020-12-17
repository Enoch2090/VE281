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
    uInt indegree = 0;
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
    };
    ~Node() {
        if (!this->next) return;
        delete this->next;
    };
    struct compareNode{
        bool operator() (Node *n1, Node *n2) { return n1->index>n2->index; }
    };
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
    vector<Node*> udAdjList;
public:
    explicit Graph(uInt size, uInt start, uInt destination, vector<Edge> v) {
        this->size = size;
        this->start = start;
        this->destination = destination;
        for (uInt i = 0; i<size; i++) { // Create Nodes
            Node* currNode = new Node();
            currNode->index = i;
            this->adjList.push_back(currNode);
            currNode = new Node();
            currNode->index = i;
            this->udAdjList.push_back(currNode);
        }
        for (auto& edge : v) { // Add connections
            adjList[edge.start]->addEdge(adjList[edge.destination], edge.weight);
            adjList[edge.destination]->indegree++;
            udAdjList[edge.start]->addEdge(adjList[edge.destination], edge.weight);
            udAdjList[edge.destination]->addEdge(adjList[edge.start], edge.weight);
        }
    };

    ~Graph() {
        for (uInt i = 0; i<size; i++) {
            delete this->adjList[i];
            delete this->udAdjList[i];
        }
    };

    void resetStatus() {
        for (uInt i=0; i<this->size; i++) {
            this->adjList[i]->dst = UINT_MAX/2;
            this->adjList[i]->visited = false;
            this->udAdjList[i]->dst = UINT_MAX/2;
            this->udAdjList[i]->visited = false;
        }
    }

    void findShortesPath() {
        resetStatus();
        this->adjList[start]->dst = 0;
        priority_queue<DNPair, vector<DNPair>, comparePair> q;
        q.push(make_pair(0, this->adjList[start]));
        while (!q.empty()) {
            DNPair top = q.top();
            q.pop();
            Node* v = top.second;
            if (v->dst != top.first) {
                continue;
            } // handle duplicates
            Node* u = v->next;
            while (u!= nullptr) {
                if (!adjList[u->index]->visited && adjList[u->index]->dst > v->dst + u->weight) {
                    adjList[u->index]->dst = v->dst + u->weight;
                    q.push(make_pair(adjList[u->index]->dst, adjList[u->index]));
                }
                u = u->next;
            }
            top.second->visited = true;
        }
        if (adjList[this->destination]->dst==UINT_MAX/2) {
            cout << "No path exists!\n";
            return;
        }
        cout << "Shortest path length is " << adjList[this->destination]->dst << "\n";
    };

    void isDAG() {
        queue<Node*> q, o;
        for (uInt i=0; i<this->size; i++) {
            if (adjList[i]->indegree==0) {
                q.push(adjList[i]);
            }
        }
        while (!q.empty()) {
            Node* currNode = q.front();
            q.pop();
            o.push(currNode);
            Node* neighborNode = currNode->next;
            while(neighborNode) {
                adjList[neighborNode->index]->indegree--;
                neighborNode = neighborNode->next;
            }
            neighborNode = currNode->next;
            while(neighborNode) {
                if (adjList[neighborNode->index]->indegree==0) { q.push(adjList[neighborNode->index]); }
                neighborNode = neighborNode->next;
            }
        }
        if (o.size()!=this->size){
            cout << "The graph is not a DAG\n";
            return;
        }
        cout << "The graph is a DAG\n";
    };

    void findMST() {
        resetStatus();
        uInt weightSum = 0;
        this->udAdjList[start]->dst = 0;
        priority_queue<DNPair, vector<DNPair>, comparePair> q;
        q.push(make_pair(0, this->udAdjList[start]));
        while (!q.empty()) {
            DNPair top = q.top();
            q.pop();
            Node* v = top.second;
            if (v->dst != top.first) {
                continue;
            } // handle duplicates
            Node* u = v->next;
            while (u!= nullptr) {
                if (!udAdjList[u->index]->visited && udAdjList[u->index]->dst > u->weight) {
                    udAdjList[u->index]->dst = u->weight;
                    q.push(make_pair(udAdjList[u->index]->dst, udAdjList[u->index]));
                    weightSum+=u->weight;
                }
                u = u->next;
            }
            top.second->visited = true;
        }
        bool allVisited = true;
        for (uInt i=0; i<this->size; i++) {
            allVisited &= this->udAdjList[i]->visited;
        }
        if (!allVisited) {
            cout << "No MST exists!";
            return;
        }
        cout << "The total weight of MST is " << weightSum << "\n";
    }
};


int main() {
    uInt size, start, destination;
    cin >> size >> start >> destination;
    string str;
    istringstream iStream;
    vector<Edge> e;
    uInt edgeStart, edgeDest, edgeWeight;
    while (!cin.eof()) {
        cin >> edgeStart >> edgeDest >> edgeWeight;
        e.push_back(Edge(edgeStart, edgeDest, edgeWeight));
    }
    Graph g = Graph(size, start, destination, e);
    g.findShortesPath();
    g.isDAG();
    g.findMST();
}
//getline(cin, str);
//iStream.str(str);
//iStream