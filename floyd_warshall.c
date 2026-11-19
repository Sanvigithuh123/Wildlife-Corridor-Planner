
#include <stdio.h>
#include <limits.h>
#include "floyd_warshall.h"
#include "data_loader.h"

static int findHabitatIndex(
    Habitat habitats[],
    int n,
    int id
) {
    for (int i = 0; i < n; i++) {
        if (habitats[i].id == id)
            return i;
    }

    return -1;
}

void floydWarshall(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount
) {
    if (habitatCount <= 0 ||
        habitatCount > MAX_HABITATS) {
        printf("Invalid habitat count.\n");
        return;
    }

    const long long INF = LLONG_MAX / 4;

    long long dist[MAX_HABITATS][MAX_HABITATS];

    // Initialize distance matrix
    for (int i = 0; i < habitatCount; i++) {
        for (int j = 0; j < habitatCount; j++) {
            dist[i][j] = (i == j) ? 0 : INF;
        }
    }

    // Load available corridors
    for (int i = 0; i < corridorCount; i++) {

        if (!corridors[i].available)
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
            printf("Invalid corridor endpoint.\n");
            return;
        }

        long long weight = corridors[i].cost;

        if (weight < dist[u][v]) {
            dist[u][v] = weight;
            dist[v][u] = weight;
        }
    }

    // Floyd-Warshall dynamic programming
    for (int k = 0; k < habitatCount; k++) {

        for (int i = 0; i < habitatCount; i++) {

            for (int j = 0; j < habitatCount; j++) {

                if (dist[i][k] == INF ||
                    dist[k][j] == INF)
                    continue;

                long long throughK =
                    dist[i][k] + dist[k][j];

                if (throughK < dist[i][j]) {
                    dist[i][j] = throughK;
                }
            }
        }
    }

    // Detect negative cycles
    for (int i = 0; i < habitatCount; i++) {
        if (dist[i][i] < 0) {
            printf("Negative-weight cycle detected.\n");
            return;
        }
    }

    printf("\n===== FLOYD-WARSHALL =====\n");
    printf("All-Pairs Minimum Route Costs:\n\n");

    printf("%12s", "");

    for (int i = 0; i < habitatCount; i++) {
        printf("%12d", habitats[i].id);
    }

    printf("\n");

    for (int i = 0; i < habitatCount; i++) {

        printf("%12d", habitats[i].id);

        for (int j = 0; j < habitatCount; j++) {

            if (dist[i][j] == INF)
                printf("%12s", "INF");
            else
                printf("%12lld", dist[i][j]);
        }

        printf("\n");
    }
}
