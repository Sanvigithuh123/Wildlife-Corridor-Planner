
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "knapsack.h"

int loadInterventions(
    const char *filename,
    Intervention items[],
    int maxItems
) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Cannot open %s\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (count >= maxItems) {
            printf("Too many interventions.\n");
            fclose(fp);
            return -1;
        }

        Intervention item;

        int fields = sscanf(
            line,
            "%d,%49[^,],%d,%d",
            &item.id,
            item.name,
            &item.cost,
            &item.benefit
        );

        if (fields != 4 ||
            item.id < 0 ||
            item.cost <= 0 ||
            item.benefit < 0) {
            printf("Invalid intervention record.\n");
            fclose(fp);
            return -1;
        }

        for (int i = 0; i < count; i++) {
            if (items[i].id == item.id) {
                printf("Duplicate intervention ID.\n");
                fclose(fp);
                return -1;
            }
        }

        items[count++] = item;
    }

    fclose(fp);
    return count;
}

int calculateKnapsack(
    const Intervention items[],
    int n,
    int budget,
    BudgetResult *out
) {
    if (!items || !out || n < 0 ||
        n > MAX_INTERVENTIONS ||
        budget < 0 || budget > 10000)
        return -1;

    *out = (BudgetResult){0};

    size_t cols = (size_t)budget + 1;
    size_t rows = (size_t)n + 1;

    long long *dp = calloc(
        rows * cols, sizeof(*dp)
    );
    if (!dp) return -1;

    #define DP(i,w) dp[(size_t)(i)*cols+(size_t)(w)]

    for (int i = 1; i <= n; i++) {
        int cost = items[i - 1].cost;
        int benefit = items[i - 1].benefit;

        if (cost <= 0 || benefit < 0) {
            free(dp);
            return -1;
        }

        for (int w = 0; w <= budget; w++) {
            DP(i,w) = DP(i-1,w);

            if (cost <= w &&
                DP(i-1,w-cost) + benefit > DP(i,w)) {
                DP(i,w) = DP(i-1,w-cost) + benefit;
            }
        }
    }

    out->maxBenefit = DP(n,budget);
    int w = budget;

    for (int i = n; i > 0; i--) {
        if (DP(i,w) != DP(i-1,w)) {
            out->selected[i-1] = 1;
            out->selectedCount++;
            out->totalCost += items[i-1].cost;
            w -= items[i-1].cost;
        }
    }

    free(dp);
    #undef DP
    return 0;
}

void optimizeBudget(
    Intervention items[],
    int n,
    int budget
) {
    BudgetResult r;

    if (calculateKnapsack(items, n, budget, &r)) {
        puts("Invalid budget, items, or allocation failure.");
        return;
    }

    puts("\n===== CONSERVATION BUDGET OPTIMIZATION =====");
    printf("Available Budget: %d lakh\n", budget);
    printf("Maximum Benefit: %lld\n", r.maxBenefit);
    puts("\nSelected Interventions:");

    for (int i = 0; i < n; i++) {
        if (r.selected[i]) {
            printf("%s | Cost: %d | Benefit: %d\n",
                items[i].name,
                items[i].cost,
                items[i].benefit);
        }
    }

    printf("\nTotal Budget Used: %d lakh\n",
           r.totalCost);
    printf("Remaining Budget: %d lakh\n",
           budget - r.totalCost);
}
