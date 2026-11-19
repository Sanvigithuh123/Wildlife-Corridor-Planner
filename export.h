
#ifndef EXPORT_H
#define EXPORT_H

#include "habitat.h"

int exportCSV(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
);

int exportJSON(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
);

#endif
