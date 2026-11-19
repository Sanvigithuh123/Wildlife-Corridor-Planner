
#include <stdio.h>
#include "subset_sum.h"

static int searchSubset(
    const Intervention items[],
    int n,
    int index,
    int remaining,
    int selected[]
) {
    if (remaining == 0)
        return 1;

    if (index == n || remaining < 0)
        return 0;

    // Choice 1: Include the intervention
    if (items[index].cost <= remaining) {
        selected[index] = 1;

        if (searchSubset(
            items, n, index + 1,
            remaining - items[index].cost,
            selected
        )) {
            return 1;
        }
    }

    // Choice 2: Exclude the intervention
    selected[index] = 0;

    return searchSubset(
        items, n, index + 1,
        remaining, selected
    );
}

void subsetSum(
    const Intervention items[],
    int n,
    int target
) {
    if (n < 0 || n > MAX_INTERVENTIONS ||
        target < 0) {
        printf("Invalid subset sum input.\n");
        return;
    }

    int selected[MAX_INTERVENTIONS] = {0};

    printf("\n===== SUBSET SUM =====\n");
    printf("Target Budget: %d lakh\n", target);

    if (searchSubset(
        items, n, 0, target, selected
    )) {
        printf("Exact budget combination found:\n");

        for (int i = 0; i < n; i++) {
            if (selected[i]) {
                printf(
                    "%s | Cost: %d\n",
                    items[i].name,
                    items[i].cost
                );
            }
        }
    }
    else {
        printf("No exact budget combination exists.\n");
    }
}
