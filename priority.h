
#ifndef PRIORITY_H
#define PRIORITY_H

#include "habitat.h"

typedef struct {
    Habitat habitat;
    double score;
} HabitatPriority;

void rankHabitats(
    const Habitat habitats[],
    int habitatCount
);

#endif
