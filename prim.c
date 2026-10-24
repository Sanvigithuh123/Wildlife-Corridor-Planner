#include <stdio.h>
#include <limits.h>
#include "prim.h"
#include "data_loader.h"

// Convert a habitat ID to its array index
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

void primMST(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount
) {
    if (habitatCount <= 0 ||
        habitatCount > MAX_HABITATS) {
        printf("Invalid number of habitats.\n");
        return;
    }

    int graph[MAX_HABITATS][MAX_HABITATS];

    // Initialize adjacency matrix
    for (int i = 0; i < habitatCount; i++) {
        for (int j = 0; j < habitatCount; j++) {
            graph[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }

    // Convert corridor records into matrix edges
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

        if (u == -1 || v == -1) {
            printf("Invalid corridor habitat ID.\n");
            return;
        }

        int cost = corridors[i].cost;

        // Keep the cheapest edge if duplicates exist
        if (cost < graph[u][v]) {
            graph[u][v] = cost;
            graph[v][u] = cost;
        }
    }

    int key[MAX_HABITATS];
    int parent[MAX_HABITATS];
    int visited[MAX_HABITATS] = {0};

    for (int i = 0; i < habitatCount; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }

    // Start from the first habitat
    key[0] = 0;

    for (int count = 0; count < habitatCount; count++) {

        int u = -1;
        int minimum = INT_MAX;

        // Find unvisited vertex with minimum key
        for (int v = 0; v < habitatCount; v++) {
            if (!visited[v] && key[v] < minimum) {
                minimum = key[v];
                u = v;
            }
        }

        if (u == -1) {
            printf("\nGraph is disconnected.\n");
            printf("A complete MST does not exist.\n");
            return;
        }

        visited[u] = 1;

        // Update minimum costs for neighbors
        for (int v = 0; v < habitatCount; v++) {

            if (!visited[v] &&
                graph[u][v] < key[v]) {

                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

    long long totalCost = 0;

    printf("\n===== PRIM MST =====\n");
    printf("Selected Wildlife Corridors:\n");

    for (int i = 1; i < habitatCount; i++) {

        int p = parent[i];

        if (p == -1) {
            printf("Incomplete spanning tree.\n");
            return;
        }

        printf(
            "%s -> %s | Cost: %d\n",
            habitats[p].name,
            habitats[i].name,
            graph[p][i]
        );

        totalCost += graph[p][i];
    }

    printf("\nTotal Minimum Cost: %lld\n", totalCost);
    printf("All habitats connected successfully.\n");
}
