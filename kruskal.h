
#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "habitat.h"

#define MAX_MST_EDGES 100

typedef struct {
    Corridor selected[MAX_MST_EDGES];
    int selectedCount;
    long long totalCost;
    int connected;
} MSTResult;

int calculateKruskalMST(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount,
    MSTResult *result
);

void kruskalMST(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount
);

#endif
