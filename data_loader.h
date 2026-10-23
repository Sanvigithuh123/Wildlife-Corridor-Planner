
#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "habitat.h"

#define MAX_HABITATS 100
#define MAX_CORRIDORS 1000

int loadHabitats(
    const char *filename,
    Habitat habitats[],
    int maxHabitats
);

int loadCorridors(
    const char *filename,
    Corridor corridors[],
    int maxCorridors
);

#endif
