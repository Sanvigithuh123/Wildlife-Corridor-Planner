
#ifndef TSP_H
#define TSP_H

#include "habitat.h"

// Limit exact TSP to small instances
#define MAX_TSP_HABITATS 15

void solveTSP(
    Habitat habitats[],
    int habitatCount,
    Corridor corridors[],
    int corridorCount
);

#endif
