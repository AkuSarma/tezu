#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define MAX 100

using namespace std;

int adjMatrix[MAX][MAX];
int visited[MAX];
int color[MAX];
int nVertices = 0;

// Function to read graph from adjacency list file and build adjacency matrix
void readGraphFromFile(const char *filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    string line;
    int row = 0;
    while (getline(file, line))
    {
        row++;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ','))
        {
            int col = atoi(token.c_str());
            if (col > 0 && col < MAX)
            {
                adjMatrix[row][col] = 1;
            }
        }
    }
    file.close();
    nVertices = row;
}

// Function to display adjacency matrix
void displayMatrix()
{
    cout << "\nAdjacency Matrix (" << nVertices << " vertices):\n";
    for (int i = 1; i <= nVertices; i++)
    {
        for (int j = 1; j <= nVertices; j++)
        {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

// DFS utility
void DFSUtil(int v)
{
    visited[v] = 1;
    cout << v << " ";

    for (int i = 1; i <= nVertices; i++)
    {
        if (adjMatrix[v][i] && !visited[i])
        {
            DFSUtil(i);
        }
    }
}

// Find connected components using DFS
void findConnectedComponents()
{
    for (int i = 1; i <= nVertices; i++)
        visited[i] = 0;

    cout << "\nConnected Components (DFS):\n";
    for (int v = 1; v <= nVertices; v++)
    {
        if (!visited[v])
        {
            cout << "{ ";
            DFSUtil(v);
            cout << "}\n";
        }
    }
}

// BFS to check bipartiteness
int isBipartite(int start)
{
    for (int i = 1; i <= nVertices; i++)
    {
        color[i] = -1;
    }

    int queue[MAX], front = 0, rear = 0;
    queue[rear++] = start;
    color[start] = 0;

    while (front < rear)
    {
        int u = queue[front++];
        for (int v = 1; v <= nVertices; v++)
        {
            if (adjMatrix[u][v])
            {
                if (color[v] == -1)
                {
                    color[v] = 1 - color[u];
                    queue[rear++] = v;
                }
                else if (color[v] == color[u])
                {
                    return 0; // Not bipartite
                }
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    readGraphFromFile(argv[1]);
    displayMatrix();

    int choice, start;
    srand(time(NULL));

    cout << "\nEnter starting vertex for DFS (0 for random): ";
    cin >> start;
    if (start == 0)
        start = rand() % nVertices + 1;
    cout << "Starting DFS from vertex " << start << endl;

    for (int i = 1; i <= nVertices; i++)
        visited[i] = 0;

    cout << "DFS traversal from " << start << ": ";
    DFSUtil(start);
    cout << endl;

    findConnectedComponents();

    cout << "\nEnter starting vertex for BFS (0 for random): ";
    cin >> start;
    if (start == 0)
        start = rand() % nVertices + 1;
    cout << "Starting BFS from vertex " << start << endl;

    if (isBipartite(start))
        cout << "Graph is Bipartite." << endl;
    else
        cout << "Graph is NOT Bipartite." << endl;

    return 0;
}
