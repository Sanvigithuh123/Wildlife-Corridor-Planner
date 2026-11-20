
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "scenario.h"
#include "kruskal.h"

// Find habitat index using habitat ID
static int findIndex(
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

// Disjoint Set Union - Find Root
static int findRoot(int parent[], int x) {
    if (parent[x] != x)
        parent[x] = findRoot(parent, parent[x]);

    return parent[x];
}

// Analyze habitat network
ScenarioSummary analyzeScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    ScenarioSummary result = {0};

    if (!habitats || !corridors ||
        habitatCount < 0 ||
        corridorCount < 0) {
        result.connectedComponents = -1;
        return result;
    }

    result.habitatCount = habitatCount;
    result.corridorCount = corridorCount;
    result.connectedComponents = habitatCount;

    int *parent = malloc(
        (size_t)(habitatCount > 0 ?
        habitatCount : 1) * sizeof(int)
    );

    if (parent == NULL) {
        printf("Memory allocation failed.\n");
        result.connectedComponents = -1;
        return result;
    }

    for (int i = 0; i < habitatCount; i++)
        parent[i] = i;

    for (int i = 0; i < corridorCount; i++) {

        if (!corridors[i].available)
            continue;

        int u = findIndex(
            habitats,
            habitatCount,
            corridors[i].source
        );

        int v = findIndex(
            habitats,
            habitatCount,
            corridors[i].destination
        );

        if (u == -1 || v == -1 ||
            corridors[i].capacity < 0) {
            printf("Invalid corridor data.\n");
            result.connectedComponents = -1;
            free(parent);
            return result;
        }

        result.availableCorridors++;

        if (result.totalCapacity >
            INT_MAX - corridors[i].capacity) {
            printf("Total capacity overflow.\n");
            result.connectedComponents = -1;
            free(parent);
            return result;
        }

        result.totalCapacity += corridors[i].capacity;

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

// Display scenario summary
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

// Reuse existing Kruskal algorithm
static void printMSTAnalysis(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    MSTResult result;

    int status = calculateKruskalMST(
        habitats,
        habitatCount,
        corridors,
        corridorCount,
        &result
    );

    if (status != 0) {
        printf("MST calculation failed.\n");
        return;
    }

    if (result.connected) {
        printf("Minimum Corridor Network Cost: %lld\n",
               result.totalCost);

        printf("Recommended Corridors: %d\n",
               result.selectedCount);
    }
    else {
        printf("Network is disconnected.\n");

        printf("Minimum Spanning Forest Cost: %lld\n",
               result.totalCost);

        printf("Selected Corridors: %d\n",
               result.selectedCount);
    }
}

// Scenario 1: Normal Operation
void runNormalScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
) {
    printf("\n===== NORMAL CONSERVATION SCENARIO =====\n");

    ScenarioSummary result = analyzeScenario(
        habitats,
        habitatCount,
        corridors,
        corridorCount
    );

    if (result.connectedComponents < 0) {
        printf("Scenario analysis failed.\n");
        return;
    }

    printSummary(&result);

    printf("\n===== MINIMUM COST CORRIDOR ANALYSIS =====\n");

    printMSTAnalysis(
        habitats,
        habitatCount,
        corridors,
        corridorCount
    );
}

// Scenario 2: Corridor Constraint Shock
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

    // Copy original corridor data
    for (int i = 0; i < corridorCount; i++)
        modified[i] = corridors[i];

    // Block the first available corridor
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

    // Analyze original network
    ScenarioSummary before = analyzeScenario(
        habitats,
        habitatCount,
        corridors,
        corridorCount
    );

    // Analyze modified network
    ScenarioSummary after = analyzeScenario(
        habitats,
        habitatCount,
        modified,
        corridorCount
    );

    if (before.connectedComponents < 0 ||
        after.connectedComponents < 0) {
        printf("Scenario analysis failed.\n");
        free(modified);
        return;
    }

    printf("\n===== CORRIDOR SHOCK SCENARIO =====\n");

    printf(
        "Blocked Corridor: %d -> %d\n",
        modified[blocked].source,
        modified[blocked].destination
    );

    // Before shock
    printf("\n===== BEFORE SHOCK =====\n");

    printSummary(&before);

    printf("\nMinimum Cost Corridor Analysis:\n");

    printMSTAnalysis(
        habitats,
        habitatCount,
        corridors,
        corridorCount
    );

    // After shock
    printf("\n===== AFTER SHOCK =====\n");

    printSummary(&after);

    printf("\nMinimum Cost Corridor Analysis:\n");

    printMSTAnalysis(
        habitats,
        habitatCount,
        modified,
        corridorCount
    );

    // Final comparison
    printf("\n===== SHOCK IMPACT SUMMARY =====\n");

    printf(
        "Available Corridors: %d -> %d\n",
        before.availableCorridors,
        after.availableCorridors
    );

    printf(
        "Total Edge Capacity: %d -> %d\n",
        before.totalCapacity,
        after.totalCapacity
    );

    printf(
        "Connected Components: %d -> %d\n",
        before.connectedComponents,
        after.connectedComponents
    );

    if (before.connectedComponents == 1 &&
        after.connectedComponents > 1) {

        printf(
            "WARNING: Corridor disruption "
            "disconnected the habitat network.\n"
        );
    }
    else if (after.connectedComponents == 1) {

        printf(
            "Network remains connected "
            "after the disruption.\n"
        );
    }
    else {
        printf(
            "Network remains disconnected "
            "after the disruption.\n"
        );
    }

    free(modified);
}
