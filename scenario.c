
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "scenario.h"

static int findIndex(
    const Habitat habitats[],
    int n,
    int id
) {
    for (int i = 0; i < n; i++)
        if (habitats[i].id == id)
            return i;

    return -1;
}

static int findRoot(int parent[], int x) {
    if (parent[x] != x)
        parent[x] = findRoot(parent, parent[x]);

    return parent[x];
}

ScenarioSummary analyzeScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    ScenarioSummary result = {0};

    if (habitatCount < 0 || corridorCount < 0)
        return result;

    result.habitatCount = habitatCount;
    result.corridorCount = corridorCount;
    result.connectedComponents = habitatCount;

    int *parent = malloc(
        (size_t)habitatCount * sizeof(int)
    );

    if (habitatCount > 0 && parent == NULL) {
        printf("Scenario memory allocation failed.\n");
        result.connectedComponents = -1;
        return result;
    }

    for (int i = 0; i < habitatCount; i++)
        parent[i] = i;

    for (int i = 0; i < corridorCount; i++) {

        if (!corridors[i].available)
            continue;

        int u = findIndex(
            habitats, habitatCount,
            corridors[i].source
        );

        int v = findIndex(
            habitats, habitatCount,
            corridors[i].destination
        );

        if (u == -1 || v == -1) {
            printf("Invalid corridor endpoint in scenario.\n");
            result.connectedComponents = -1;
            free(parent);
            return result;
        }

        result.availableCorridors++;

        if (corridors[i].capacity > 0 &&
            result.totalCapacity <=
            INT_MAX - corridors[i].capacity) {
            result.totalCapacity += corridors[i].capacity;
        }

        int rootU = findRoot(parent, u);
        int rootV = findRoot(parent, v);

        if (rootU != rootV) {
            parent[rootU] = rootV;
            result.connectedComponents--;
        }
    }

    free(parent);
    return result;
}

static void printSummary(
    const ScenarioSummary *s
) {
    printf("Habitats: %d\n", s->habitatCount);
    printf("Corridors: %d\n", s->corridorCount);
    printf("Available Corridors: %d\n",
           s->availableCorridors);
    printf("Connected Components: %d\n",
           s->connectedComponents);
    printf("Total Available Edge Capacity: %d\n",
           s->totalCapacity);
}

void runNormalScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    printf("\n===== NORMAL CONSERVATION SCENARIO =====\n");

    ScenarioSummary result = analyzeScenario(
        habitats, habitatCount,
        corridors, corridorCount
    );

    printSummary(&result);
}

void runShockScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    if (corridorCount <= 0) {
        printf("No corridors available for shock analysis.\n");
        return;
    }

    Corridor *modified = malloc(
        (size_t)corridorCount * sizeof(Corridor)
    );

    if (modified == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < corridorCount; i++)
        modified[i] = corridors[i];

    // Simulate blocking the first available corridor
    int blocked = -1;

    for (int i = 0; i < corridorCount; i++) {
        if (modified[i].available) {
            modified[i].available = 0;
            blocked = i;
            break;
        }
    }

    if (blocked == -1) {
        printf("No available corridor to block.\n");
        free(modified);
        return;
    }

    ScenarioSummary before = analyzeScenario(
        habitats, habitatCount,
        corridors, corridorCount
    );

    ScenarioSummary after = analyzeScenario(
        habitats, habitatCount,
        modified, corridorCount
    );

    printf("\n===== CORRIDOR SHOCK SCENARIO =====\n");

    printf(
        "Blocked Corridor: %d -> %d\n",
        modified[blocked].source,
        modified[blocked].destination
    );

    printf("\nBefore Shock:\n");
    printSummary(&before);

    printf("\nAfter Shock:\n");
    printSummary(&after);

    free(modified);
}
