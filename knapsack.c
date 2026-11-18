
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

void optimizeBudget(
    Intervention items[],
    int n,
    int budget
) {
    if (n <= 0 || n > MAX_INTERVENTIONS ||
        budget < 0 || budget > 10000) {
        printf("Invalid budget or item count.\n");
        return;
    }

    // Allocate DP table dynamically
    long long **dp = malloc(
        (size_t)(n + 1) * sizeof(long long *)
    );

    if (dp == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i <= n; i++) {

        dp[i] = calloc(
            (size_t)(budget + 1),
            sizeof(long long)
        );

        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(dp[j]);

            free(dp);
            printf("Memory allocation failed.\n");
            return;
        }
    }

    // Build DP table
    for (int i = 1; i <= n; i++) {

        int cost = items[i - 1].cost;
        int benefit = items[i - 1].benefit;

        for (int w = 0; w <= budget; w++) {

            // Do not select current item
            dp[i][w] = dp[i - 1][w];

            // Select item if it fits
            if (cost <= w) {

                long long take =
                    benefit + dp[i - 1][w - cost];

                if (take > dp[i][w])
                    dp[i][w] = take;
            }
        }
    }

    printf("\n===== CONSERVATION BUDGET OPTIMIZATION =====\n");
    printf("Available Budget: %d lakh\n", budget);
    printf("Maximum Benefit: %lld\n", dp[n][budget]);

    // Reconstruct selected interventions
    int selected[MAX_INTERVENTIONS] = {0};
    int w = budget;
    int totalCost = 0;

    for (int i = n; i > 0; i--) {

        if (dp[i][w] != dp[i - 1][w]) {

            selected[i - 1] = 1;
            w -= items[i - 1].cost;
            totalCost += items[i - 1].cost;
        }
    }

    printf("\nSelected Interventions:\n");

    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf(
                "%s | Cost: %d | Benefit: %d\n",
                items[i].name,
                items[i].cost,
                items[i].benefit
            );
        }
    }

    printf("\nTotal Budget Used: %d lakh\n", totalCost);
    printf("Remaining Budget: %d lakh\n", budget - totalCost);

    for (int i = 0; i <= n; i++)
        free(dp[i]);

    free(dp);
}
