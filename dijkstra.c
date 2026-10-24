#include <stdio.h>
#include <limits.h>
#include "dijkstra.h"
#include "data_loader.h"

// Find the array index corresponding to a habitat ID
static int findHabitatIndex(
    Habitat habitats[],
    int count,
    int id
) {
    for (int i = 0; i < count; i++) {
        if (habitats[i].id == id)
            return i;
    }

    return -1;
}

void dijkstra(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount,
    int sourceId,
    int destinationId
) {
    if (habitatCount <= 0 ||
        habitatCount > MAX_HABITATS) {
        printf("Invalid habitat count.\n");
        return;
    }

    int source = findHabitatIndex(
        habitats, habitatCount, sourceId
    );

    int destination = findHabitatIndex(
        habitats, habitatCount, destinationId
    );

    if (source == -1 || destination == -1) {
        printf("Invalid source or destination ID.\n");
        return;
    }

    // Use long long to avoid overflow for route sums
    const long long INF = LLONG_MAX / 4;

    long long graph[MAX_HABITATS][MAX_HABITATS];

    // Initialize adjacency matrix
    for (int i = 0; i < habitatCount; i++) {
        for (int j = 0; j < habitatCount; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    // Build graph from available corridors
    for (int i = 0; i < corridorCount; i++) {

        if (corridors[i].available == 0)
            continue;

        int u = findHabitatIndex(
            habitats,
            habitatCount,
            corridors[i].source
        );

        int v = findHabitatIndex(
            habitats,
            habitatCount,
            corridors[i].destination
        );

        if (u == -1 || v == -1 ||
            corridors[i].cost < 0) {
            printf("Invalid corridor data.\n");
            return;
        }

        long long weight = corridors[i].cost;

        if (weight < graph[u][v]) {
            graph[u][v] = weight;
            graph[v][u] = weight;
        }
    }

    long long dist[MAX_HABITATS];
    int visited[MAX_HABITATS] = {0};
    int parent[MAX_HABITATS];

    for (int i = 0; i < habitatCount; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    dist[source] = 0;

    for (int count = 0; count < habitatCount; count++) {

        int u = -1;
        long long minimum = INF;

        // Find unvisited vertex with minimum distance
        for (int i = 0; i < habitatCount; i++) {
            if (!visited[i] && dist[i] < minimum) {
                minimum = dist[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        // Destination finalized
        if (u == destination)
            break;

        // Relax edges
        for (int v = 0; v < habitatCount; v++) {

            if (!visited[v] &&
                graph[u][v] != INF &&
                dist[u] + graph[u][v] < dist[v]) {

                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    printf("\n===== DIJKSTRA SHORTEST PATH =====\n");

    if (dist[destination] == INF) {
        printf("No available route between habitats.\n");
        return;
    }

    printf("Source: %s\n", habitats[source].name);
    printf("Destination: %s\n",
           habitats[destination].name);

    printf("Minimum Route Weight: %lld\n",
           dist[destination]);

    // Reconstruct path backwards
    int path[MAX_HABITATS];
    int pathLength = 0;

    for (int v = destination;
         v != -1;
         v = parent[v]) {

        path[pathLength++] = v;
    }

    printf("Route: ");

    // Print in forward order
    for (int i = pathLength - 1; i >= 0; i--) {

        printf("%s", habitats[path[i]].name);

        if (i > 0)
            printf(" -> ");
    }

    printf("\n");
}
