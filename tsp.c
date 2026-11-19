
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "tsp.h"

#define INF (LLONG_MAX / 4)

// Find habitat index using its ID
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

void solveTSP(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount
) {
    int n = habitatCount;

    if (n <= 0 || n > MAX_TSP_HABITATS) {
        printf(
            "TSP supports 1 to %d habitats.\n",
            MAX_TSP_HABITATS
        );
        return;
    }

    // Allocate shortest-path distance matrix
    long long dist[MAX_TSP_HABITATS][MAX_TSP_HABITATS];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = (i == j) ? 0 : INF;
        }
    }

    // Load available corridors
    for (int i = 0; i < corridorCount; i++) {

        if (!corridors[i].available)
            continue;

        int u = findHabitatIndex(
            habitats, n, corridors[i].source
        );

        int v = findHabitatIndex(
            habitats, n, corridors[i].destination
        );

        if (u == -1 || v == -1 ||
            corridors[i].cost < 0) {
            printf("Invalid corridor data.\n");
            return;
        }

        long long cost = corridors[i].cost;

        if (cost < dist[u][v]) {
            dist[u][v] = cost;
            dist[v][u] = cost;
        }
    }

    // Floyd-Warshall preprocessing
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                if (dist[i][k] == INF ||
                    dist[k][j] == INF)
                    continue;

                long long candidate =
                    dist[i][k] + dist[k][j];

                if (candidate < dist[i][j])
                    dist[i][j] = candidate;
            }
        }
    }

    // Check connectivity
    for (int i = 0; i < n; i++) {
        if (dist[0][i] == INF) {
            printf(
                "No complete inspection tour exists.\n"
            );
            return;
        }
    }

    int states = 1 << n;

    size_t totalStates = (size_t)states * n;

    long long *dp = malloc(
        totalStates * sizeof(long long)
    );

    int *parent = malloc(
        totalStates * sizeof(int)
    );

    if (dp == NULL || parent == NULL) {
        printf("Memory allocation failed.\n");
        free(dp);
        free(parent);
        return;
    }

    for (size_t i = 0; i < totalStates; i++) {
        dp[i] = INF;
        parent[i] = -1;
    }

    // Start from habitat 0
    dp[(size_t)1 * n] = 0;

    for (int mask = 1; mask < states; mask++) {

        if (!(mask & 1))
            continue;

        for (int u = 0; u < n; u++) {

            if (!(mask & (1 << u)))
                continue;

            long long current =
                dp[(size_t)mask * n + u];

            if (current == INF)
                continue;

            for (int v = 0; v < n; v++) {

                if (mask & (1 << v))
                    continue;

                if (dist[u][v] == INF)
                    continue;

                int nextMask = mask | (1 << v);

                long long candidate =
                    current + dist[u][v];

                size_t index =
                    (size_t)nextMask * n + v;

                if (candidate < dp[index]) {
                    dp[index] = candidate;
                    parent[index] = u;
                }
            }
        }
    }

    int fullMask = states - 1;
    long long bestCost = INF;
    int last = -1;

    // Return to the starting habitat
    for (int u = 0; u < n; u++) {

        long long current =
            dp[(size_t)fullMask * n + u];

        if (current == INF || dist[u][0] == INF)
            continue;

        long long candidate =
            current + dist[u][0];

        if (candidate < bestCost) {
            bestCost = candidate;
            last = u;
        }
    }

    if (last == -1) {
        printf("No valid TSP tour exists.\n");
        free(dp);
        free(parent);
        return;
    }

    // Reconstruct tour backwards
    int tour[MAX_TSP_HABITATS + 1];

    int mask = fullMask;
    int current = last;

    for (int pos = n - 1; pos >= 0; pos--) {

        tour[pos] = current;

        int previous =
            parent[(size_t)mask * n + current];

        mask ^= (1 << current);
        current = previous;
    }

    tour[n] = 0;

    printf("\n===== RANGER INSPECTION TOUR (TSP) =====\n");
    printf("Starting Habitat: %s\n",
           habitats[0].name);

    printf("Optimal Tour:\n");

    for (int i = 0; i <= n; i++) {

        printf("%s", habitats[tour[i]].name);

        if (i < n)
            printf(" -> ");
    }

    printf("\nMinimum Total Travel Cost: %lld\n",
           bestCost);

    free(dp);
    free(parent);
}
