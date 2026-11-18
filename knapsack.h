
#ifndef KNAPSACK_H
#define KNAPSACK_H

#define MAX_INTERVENTIONS 100

typedef struct {
    int id;
    char name[50];
    int cost;
    int benefit;
} Intervention;

int loadInterventions(
    const char *filename,
    Intervention items[],
    int maxItems
);

void optimizeBudget(
    Intervention items[],
    int n,
    int budget
);

#endif
