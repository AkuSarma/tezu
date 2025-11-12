#include <stdio.h>
#define INF 999

void dijkstra(int n, int cost[10][10], int source)
{
    int dist[10], visited[10];
    int i, j, count, min, next;

    // Initialize
    for (i = 1; i <= n; i++)
    {
        dist[i] = cost[source][i];
        visited[i] = 0;
    }
    dist[source] = 0;
    visited[source] = 1;
    count = 1;

    while (count < n - 1)
    {
        min = INF;
        for (i = 1; i <= n; i++)
        {
            if (dist[i] < min && !visited[i])
            {
                min = dist[i];
                next = i;
            }
        }

        visited[next] = 1;
        for (i = 1; i <= n; i++)
        {
            if (!visited[i])
            {
                if (min + cost[next][i] < dist[i])
                    dist[i] = min + cost[next][i];
            }
        }
        count++;
    }

    printf("\nShortest distances from node %d:\n", source);
    for (i = 1; i <= n; i++)
        if (i != source)
            printf("To node %d = %d\n", i, dist[i]);
}

int main()
{
    int n, cost[10][10], i, j, source;

    printf("Enter number of nodes: ");
    scanf("%d", &n);
    printf("Enter cost adjacency matrix (999 for no path):\n");
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            scanf("%d", &cost[i][j]);

    printf("Enter source node: ");
    scanf("%d", &source);

    dijkstra(n, cost, source);
    return 0;
}
