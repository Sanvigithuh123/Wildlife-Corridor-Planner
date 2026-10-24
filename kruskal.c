#include <stdio.h>
#include <stdlib.h>
#include "kruskal.h"

#define MAX_VERTICES 100

// Find the root of a component
int findRoot(int parent[], int x) {

    if (parent[x] != x) {
        parent[x] = findRoot(parent, parent[x]);
    }

    return parent[x];
}

// Combine two components
void unionSets(
    int parent[],
    int rank[],
    int a,
    int b
) {
    int rootA = findRoot(parent, a);
    int rootB = findRoot(parent, b);

    if (rootA == rootB)
        return;

    if (rank[rootA] < rank[rootB]) {
        parent[rootA] = rootB;
    }
    else if (rank[rootA] > rank[rootB]) {
        parent[rootB] = rootA;
    }
    else {
        parent[rootB] = rootA;
        rank[rootA]++;
    }
}

// Compare corridor costs for qsort
int compareCorridors(const void *a, const void *b) {

    const Corridor *c1 = (const Corridor *)a;
    const Corridor *c2 = (const Corridor *)b;

    return (c1->cost > c2->cost) -
           (c1->cost < c2->cost);
}

// Find the array index of a habitat ID
int getHabitatIndex(
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

void kruskalMST(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount
) {
    if (habitatCount <= 0 ||
        habitatCount > MAX_VERTICES ||
        corridorCount < 0) {
        printf("Invalid graph size.\n");
        return;
    }

    Corridor *sorted = malloc(
        (size_t)corridorCount * sizeof(Corridor)
    );

    if (corridorCount > 0 && sorted == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int edgeCount = 0;

    // Copy only available corridors
    for (int i = 0; i < corridorCount; i++) {
        if (corridors[i].available == 1) {
            sorted[edgeCount++] = corridors[i];
        }
    }

    // Sort corridors by increasing cost
    qsort(
        sorted,
        edgeCount,
        sizeof(Corridor),
        compareCorridors
    );

    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES] = {0};

    for (int i = 0; i < habitatCount; i++) {
        parent[i] = i;
    }

    int selected = 0;
    long long totalCost = 0;

    printf("\n===== KRUSKAL MST =====\n");
    printf("Selected Wildlife Corridors:\n");

    for (int i = 0; i < edgeCount; i++) {

        int u = getHabitatIndex(
            habitats,
            habitatCount,
            sorted[i].source
        );

        int v = getHabitatIndex(
            habitats,
            habitatCount,
            sorted[i].destination
        );

        if (u == -1 || v == -1) {
            printf("Unknown habitat ID in corridor.\n");
            free(sorted);
            return;
        }

        int rootU = findRoot(parent, u);
        int rootV = findRoot(parent, v);

        // Select edge only if no cycle is formed
        if (rootU != rootV) {

            unionSets(parent, rank, u, v);

            printf(
                "%s -> %s | Cost: %d\n",
                habitats[u].name,
                habitats[v].name,
                sorted[i].cost
            );

            totalCost += sorted[i].cost;
            selected++;
        }

        if (selected == habitatCount - 1)
            break;
    }

    if (selected == habitatCount - 1) {
        printf("\nTotal Minimum Cost: %lld\n",
               totalCost);
        printf("All habitats connected successfully.\n");
    }
    else {
        printf("\nAll habitats cannot be connected.\n");
        printf("Selected edges: %d\n", selected);
        printf("Forest cost: %lld\n", totalCost);
    }

    free(sorted);
}
