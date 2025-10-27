#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

int adjMatrix[MAX][MAX];
int visited[MAX];
int color[MAX];
int nVertices = 0;

// Function to read graph from adjacency list file and build adjacency matrix
void readGraphFromFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char line[256];
    int row = 0;
    while (fgets(line, sizeof(line), fp))
    {
        row++;
        char *token = strtok(line, ", ");
        while (token != NULL)
        {
            int col = atoi(token);
            if (col > 0 && col < MAX)
            {
                adjMatrix[row][col] = 1;
            }
            token = strtok(NULL, ", ");
        }
    }
    fclose(fp);
    nVertices = row;
}

// Function to display adjacency matrix
void displayMatrix()
{
    printf("\nAdjacency Matrix (%d vertices):\n", nVertices);
    for (int i = 1; i <= nVertices; i++)
    {
        for (int j = 1; j <= nVertices; j++)
        {
            printf("%d ", adjMatrix[i][j]);
        }
        printf("\n");
    }
}

// DFS utility
void DFSUtil(int v)
{
    visited[v] = 1;
    printf("%d ", v);

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

    printf("\nConnected Components (DFS):\n");
    for (int v = 1; v <= nVertices; v++)
    {
        if (!visited[v])
        {
            printf("{ ");
            DFSUtil(v);
            printf("}\n");
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
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    readGraphFromFile(argv[1]);
    displayMatrix();

    int choice, start;
    srand(time(NULL));

    printf("\nEnter starting vertex for DFS (0 for random): ");
    scanf("%d", &start);
    if (start == 0)
        start = rand() % nVertices + 1;
    printf("Starting DFS from vertex %d\n", start);

    for (int i = 1; i <= nVertices; i++)
        visited[i] = 0;
    printf("DFS traversal from %d: ", start);
    DFSUtil(start);
    printf("\n");
    findConnectedComponents();

    printf("\nEnter starting vertex for BFS (0 for random): ");
    scanf("%d", &start);
    if (start == 0)
        start = rand() % nVertices + 1;
    printf("Starting BFS from vertex %d\n", start);

    if (isBipartite(start))
        printf("Graph is Bipartite.\n");
    else
        printf("Graph is NOT Bipartite.\n");

    return 0;
}
