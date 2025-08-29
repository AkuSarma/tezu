#include <stdio.h>
#include <limits.h>

#define V 100  // Maximum number of vertices

void dijkstra(int graph[V][V], int n, int src) {
    int dist[V];
    int visited[V] = {0};

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min = INT_MAX, u = -1;

        // Find the vertex with minimum distance
        for (int v = 0; v < n; v++)
            if (!visited[v] && dist[v] <= min)
                min = dist[v], u = v;

        if (u == -1) break; // No more reachable vertices

        visited[u] = 1;

        // Update distances
        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    printf("Vertex\tDistance from Source\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\n", i, dist[i]);
}

int main() {
    int n, src;
    int graph[V][V];

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (0 for no edge):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    printf("Enter source vertex (0 to %d): ", n - 1);
    scanf("%d", &src);

    dijkstra(graph, n, src);

    return 0;
}