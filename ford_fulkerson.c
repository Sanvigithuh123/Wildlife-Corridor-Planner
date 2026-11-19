
#include <stdio.h>
#include <limits.h>
#include "ford_fulkerson.h"
#include "data_loader.h"

static int findHabitatIndex(
    const Habitat habitats[],
    int n,
    int id
) {
    for (int i = 0; i < n; i++) {
        if (habitats[i].id == id)
            return i;
    }
    return -1;
}

// BFS finds an augmenting path
static int bfs(
    int n,
    long long residual[MAX_HABITATS][MAX_HABITATS],
    int source,
    int sink,
    int parent[]
) {
    int visited[MAX_HABITATS] = {0};
    int queue[MAX_HABITATS];

    int front = 0;
    int rear = 0;

    queue[rear++] = source;
    visited[source] = 1;
    parent[source] = -1;

    while (front < rear) {
        int u = queue[front++];

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residual[u][v] > 0) {
                visited[v] = 1;
                parent[v] = u;

                if (v == sink)
                    return 1;

                queue[rear++] = v;
            }
        }
    }

    return 0;
}

void fordFulkerson(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount,
    int sourceId,
    int destinationId
) {
    if (habitatCount <= 0 ||
        habitatCount > MAX_HABITATS ||
        corridorCount < 0) {
        printf("Invalid graph size.\n");
        return;
    }

    int source = findHabitatIndex(
        habitats, habitatCount, sourceId
    );

    int sink = findHabitatIndex(
        habitats, habitatCount, destinationId
    );

    if (source == -1 || sink == -1) {
        printf("Invalid source or destination ID.\n");
        return;
    }

    if (source == sink) {
        printf("Source and destination must differ.\n");
        return;
    }

    long long residual[MAX_HABITATS][MAX_HABITATS] = {{0}};

    // Build directed capacity network
    for (int i = 0; i < corridorCount; i++) {
        if (!corridors[i].available)
            continue;

        int u = findHabitatIndex(
            habitats, habitatCount,
            corridors[i].source
        );

        int v = findHabitatIndex(
            habitats, habitatCount,
            corridors[i].destination
        );

        if (u == -1 || v == -1 ||
            corridors[i].capacity < 0) {
            printf("Invalid corridor capacity data.\n");
            return;
        }

        if (residual[u][v] >
            LLONG_MAX - corridors[i].capacity) {
            printf("Capacity overflow.\n");
            return;
        }

        residual[u][v] += corridors[i].capacity;
    }

    int parent[MAX_HABITATS];
    long long maxFlow = 0;
    int pathCount = 0;

    while (bfs(
        habitatCount, residual,
        source, sink, parent
    )) {
        long long pathFlow = LLONG_MAX;

        // Find bottleneck capacity
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            if (residual[u][v] < pathFlow)
                pathFlow = residual[u][v];
        }

        if (maxFlow > LLONG_MAX - pathFlow) {
            printf("Flow overflow.\n");
            return;
        }

        // Update residual capacities
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
        pathCount++;

        printf(
            "Augmenting Path %d | Added Flow: %lld\n",
            pathCount, pathFlow
        );
    }

    printf("\n===== FORD-FULKERSON MAXIMUM FLOW =====\n");
    printf("Source: %s\n", habitats[source].name);
    printf("Destination: %s\n", habitats[sink].name);
    printf("Maximum Flow: %lld\n", maxFlow);
    printf("Augmenting Paths Found: %d\n", pathCount);
}
