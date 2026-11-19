
#include <stdio.h>
#include <limits.h>
#include "bellman_ford.h"
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

static int relax(
    long long dist[],
    int parent[],
    int u,
    int v,
    long long weight,
    long long INF
) {
    if (dist[u] == INF)
        return 0;

    if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        parent[v] = u;
        return 1;
    }

    return 0;
}

void bellmanFord(
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
        printf("Invalid habitat ID.\n");
        return;
    }

    const long long INF = LLONG_MAX / 4;

    long long dist[MAX_HABITATS];
    int parent[MAX_HABITATS];

    for (int i = 0; i < habitatCount; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    dist[source] = 0;

    // Relax all available corridors V-1 times
    for (int pass = 1; pass < habitatCount; pass++) {

        int updated = 0;

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

            // Undirected corridor: relax both directions
            updated |= relax(
                dist, parent, u, v, weight, INF
            );

            updated |= relax(
                dist, parent, v, u, weight, INF
            );
        }

        if (!updated)
            break;
    }

    // Check for negative cycles
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

        if ((dist[u] != INF &&
             dist[u] + weight < dist[v]) ||
            (dist[v] != INF &&
             dist[v] + weight < dist[u])) {

            printf("Reachable negative-weight cycle detected.\n");
            return;
        }
    }

    printf("\n===== BELLMAN-FORD =====\n");

    if (dist[destination] == INF) {
        printf("No route available.\n");
        return;
    }

    printf("Source: %s\n", habitats[source].name);
    printf("Destination: %s\n",
           habitats[destination].name);

    printf("Minimum Route Weight: %lld\n",
           dist[destination]);

    int path[MAX_HABITATS];
    int length = 0;

    for (int v = destination;
         v != -1 && length < habitatCount;
         v = parent[v]) {

        path[length++] = v;
    }

    printf("Route: ");

    for (int i = length - 1; i >= 0; i--) {
        printf("%s", habitats[path[i]].name);

        if (i > 0)
            printf(" -> ");
    }

    printf("\n");
}
