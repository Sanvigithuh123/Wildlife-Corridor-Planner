
#include <stdio.h>
#include "recommendations.h"
#include "kruskal.h"

int exportRecommendations(
    const Habitat h[],
    int n,
    const Corridor c[],
    int m,
    const Intervention items[],
    int count,
    int budget
) {
    MSTResult mst;
    BudgetResult opt;

    if (calculateKruskalMST(h, n, c, m, &mst) ||
        calculateKnapsack(items, count, budget, &opt)) {
        puts("Unable to calculate recommendations.");
        return -1;
    }

    FILE *a = fopen(
        "output/recommended_corridors.csv", "w"
    );

    if (!a) {
        perror("output/recommended_corridors.csv");
        return -1;
    }

    fprintf(a,
        "source,destination,cost,capacity,available\n"
    );

    for (int i = 0; i < mst.selectedCount; i++) {
        Corridor e = mst.selected[i];

        fprintf(a, "%d,%d,%d,%d,%d\n",
            e.source,
            e.destination,
            e.cost,
            e.capacity,
            e.available);
    }

    fclose(a);

    FILE *b = fopen(
        "output/recommended_interventions.csv", "w"
    );

    if (!b) {
        perror("output/recommended_interventions.csv");
        return -1;
    }

    fprintf(b, "id,name,cost,benefit\n");

    for (int i = 0; i < count; i++) {
        if (opt.selected[i]) {
            fprintf(b, "%d,%s,%d,%d\n",
                items[i].id,
                items[i].name,
                items[i].cost,
                items[i].benefit);
        }
    }

    fclose(b);

    printf(
        "Recommended corridors: %d | MST cost: %lld | Connected: %s\n",
        mst.selectedCount,
        mst.totalCost,
        mst.connected ? "yes" : "no"
    );

    printf(
        "Budget used: %d/%d | Maximum benefit: %lld\n",
        opt.totalCost,
        budget,
        opt.maxBenefit
    );

    puts(
        "Saved: output/recommended_corridors.csv and "
        "output/recommended_interventions.csv"
    );

    return 0;
}
