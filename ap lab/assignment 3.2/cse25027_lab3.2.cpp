#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <iomanip>
using namespace std;

// ---------------------- STRUCTURES ----------------------
struct Edge {
    int src, dest, weight;
};

// For Prim's priority queue
struct Node {
    int vertex;
    int key;
    bool operator>(const Node& other) const {
        return key > other.key; // Min-heap
    }
};

// ---------------------- KRUSKAL'S ALGORITHM ----------------------
struct Subset {
    int parent, rank;
};

int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void Union(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

void kruskalMST(const vector<vector<int>>& graph, int V) {
    vector<Edge> edges;

    // Build edge list
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0)
                edges.push_back({i, j, graph[i][j]});
        }
    }

    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.weight < b.weight;
    });

    vector<Edge> result;
    Subset* subsets = new Subset[V];

    for (int v = 0; v < V; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int e = 0; // number of edges in MST
    int i = 0; // index variable for sorted edges

    cout << "\n=== Applying Kruskal's Algorithm ===\n";

    while (e < V - 1 && i < edges.size()) {
        Edge next = edges[i++];
        int x = find(subsets, next.src);
        int y = find(subsets, next.dest);

        if (x != y) {
            result.push_back(next);
            Union(subsets, x, y);
            e++;
            cout << "Edge selected: " << next.src << " - " << next.dest << " (Weight: " << next.weight << ")\n";
        }
    }

    cout << "\nMST using Kruskal's Algorithm:\n";
    int totalWeight = 0;
    for (auto edge : result) {
        cout << edge.src << " - " << edge.dest << " \tWeight: " << edge.weight << endl;
        totalWeight += edge.weight;
    }
    cout << "Total weight of MST: " << totalWeight << endl;

    delete[] subsets;
}

// ---------------------- PRIM'S ALGORITHM (with Queue Display) ----------------------
void displayPriorityQueue(priority_queue<Node, vector<Node>, greater<Node>> pq) {
    cout << "Priority Queue (vertex : key): ";
    vector<pair<int,int>> temp;
    while (!pq.empty()) {
        temp.push_back({pq.top().vertex, pq.top().key});
        pq.pop();
    }
    for (auto &p : temp)
        cout << "(" << p.first << ":" << p.second << ") ";
    cout << endl;
}

void primMST(const vector<vector<int>>& graph, int V) {
    vector<int> key(V, INT_MAX);
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);

    priority_queue<Node, vector<Node>, greater<Node>> pq;

    key[0] = 0;
    pq.push({0, 0});

    cout << "\n=== Applying Prim's Algorithm ===\n";

    while (!pq.empty()) {
        Node minNode = pq.top();
        pq.pop();

        int u = minNode.vertex;

        if (inMST[u])
            continue;

        inMST[u] = true;

        cout << "\nSelected vertex: " << u << " (Key: " << minNode.key << ")\n";
        displayPriorityQueue(pq);

        // Update keys of adjacent vertices
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
                pq.push({v, key[v]});
            }
        }
    }

    cout << "\nMST using Prim's Algorithm:\n";
    int totalWeight = 0;
    for (int i = 1; i < V; i++) {
        cout << parent[i] << " - " << i << " \tWeight: " << graph[i][parent[i]] << endl;
        totalWeight += graph[i][parent[i]];
    }
    cout << "Total weight of MST: " << totalWeight << endl;
}


// ---------- Main Function ----------
int main() {

    vector<vector<int>> graph1 = {
        {0, 4, 0, 0, 8, 0, 0, 0, 0},
        {4, 0, 8, 0, 11, 0, 0, 0, 0},
        {0, 8, 0, 7, 0, 2, 0, 4, 0},
        {0, 0, 7, 0, 0, 0, 0, 14, 9},
        {8, 11, 0, 0, 0, 7, 1, 0, 0},
        {0, 0, 2, 0, 7, 0, 6, 0, 0},
        {0, 0, 0, 0, 1, 6, 0, 2, 0},
        {0, 0, 4, 0, 14, 0, 2, 0, 10},
        {0, 0, 0, 9, 0, 0, 0, 10, 0}
    };
    vector<vector<int>> graph2 = {
        {0, 1, 0, 0, 1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0},
        {1, 1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 1, 0} 
    };

    int V1 = graph1.size();
    int V2 = graph2.size();

    cout<<"Which graph would you like to run the algorithms on?\n 1: First Graph\n 2: Second Graph\nEnter 1 or 2: ";
    if(cin.get()=='1'){
        cout<<"Running on first graph."<<endl;
        primMST(graph1, V1);
        kruskalMST(graph1, V1);
    }
    else{
        cout<<"Running on second graph."<<endl;
        primMST(graph2, V2);
        kruskalMST(graph2, V2);
    }

    return 0;
}
