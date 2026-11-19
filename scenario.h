
#ifndef SCENARIO_H
#define SCENARIO_H

#include "habitat.h"

typedef struct {
    int habitatCount;
    int corridorCount;
    int availableCorridors;
    int connectedComponents;
    int totalCapacity;
} ScenarioSummary;

ScenarioSummary analyzeScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
);

void runNormalScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
);

void runShockScenario(
    const Habitat habitats[],
    int habitatCount,
    const Corridor corridors[],
    int corridorCount
);

#endif
