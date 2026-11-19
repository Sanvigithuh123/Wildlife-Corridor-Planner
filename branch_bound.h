
#ifndef BRANCH_BOUND_H
#define BRANCH_BOUND_H

#include "knapsack.h"

#define BB_FIFO 1
#define BB_LIFO 2
#define BB_BEST_BOUND 3

void branchBoundKnapsack(
    const Intervention items[],
    int n,
    int budget,
    int method
);

#endif
