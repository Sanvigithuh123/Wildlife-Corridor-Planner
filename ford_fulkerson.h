
#ifndef FORD_FULKERSON_H
#define FORD_FULKERSON_H

#include "habitat.h"

void fordFulkerson(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount,
    int sourceId,
    int destinationId
);

#endif
