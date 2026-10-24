#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "habitat.h"

void dijkstra(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount,
    int sourceId,
    int destinationId
);

#endif
