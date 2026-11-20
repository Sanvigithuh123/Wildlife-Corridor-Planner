
#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include "habitat.h"
#include "knapsack.h"

int exportRecommendations(
    const Habitat h[],
    int n,
    const Corridor c[],
    int m,
    const Intervention items[],
    int count,
    int budget
);

#endif
