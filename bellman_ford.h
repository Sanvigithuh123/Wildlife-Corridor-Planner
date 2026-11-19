
#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "habitat.h"

void bellmanFord(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount,
    int sourceId,
    int destinationId
);

#endif
