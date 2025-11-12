#include <stdio.h>
#define INF 999

int main()
{
    int cost[10][10], dist[10][10], n, i, j, k;

    printf("Enter number of nodes: ");
    scanf("%d", &n);

    printf("Enter cost matrix (999 for no path):\n");
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            scanf("%d", &cost[i][j]);

    // Initialize distance matrix
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            dist[i][j] = cost[i][j];

    // Bellman-Ford logic
    for (k = 1; k <= n; k++)
    {
        for (i = 1; i <= n; i++)
        {
            for (j = 1; j <= n; j++)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    printf("\nDistance Vector Routing Table:\n");
    for (i = 1; i <= n; i++)
    {
        printf("From router %d:\n", i);
        for (j = 1; j <= n; j++)
        {
            if (dist[i][j] == INF)
                printf("To %d: ∞\n", j);
            else
                printf("To %d: %d\n", j, dist[i][j]);
        }
        printf("\n");
    }

    return 0;
}
