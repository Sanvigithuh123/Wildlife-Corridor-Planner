
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "benchmark.h"
#include "sorting.h"

#define BENCHMARK_RUNS 1000

static double elapsedMs(
    clock_t start,
    clock_t end
) {
    return 1000.0 * (double)(end - start)
           / CLOCKS_PER_SEC;
}

void benchmarkSorting(
    const Habitat habitats[],
    int habitatCount
) {
    if (habitatCount <= 0) {
        printf("No habitats available.\n");
        return;
    }

    Habitat *copy = malloc(
        (size_t)habitatCount * sizeof(Habitat)
    );

    if (copy == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    clock_t start = clock();

    for (int run = 0; run < BENCHMARK_RUNS; run++) {
        memcpy(
            copy,
            habitats,
            (size_t)habitatCount * sizeof(Habitat)
        );

        mergeSort(copy, habitatCount, SORT_BY_THREAT);
    }

    clock_t end = clock();

    double mergeTime =
        elapsedMs(start, end) / BENCHMARK_RUNS;

    start = clock();

    for (int run = 0; run < BENCHMARK_RUNS; run++) {
        memcpy(
            copy,
            habitats,
            (size_t)habitatCount * sizeof(Habitat)
        );

        quickSort(copy, habitatCount, SORT_BY_THREAT);
    }

    end = clock();

    double quickTime =
        elapsedMs(start, end) / BENCHMARK_RUNS;

    printf("\n===== SORTING BENCHMARK =====\n");
    printf("Habitats: %d\n", habitatCount);
    printf("Runs: %d\n", BENCHMARK_RUNS);
    printf("Merge Sort: %.6f ms/run\n", mergeTime);
    printf("Quick Sort: %.6f ms/run\n", quickTime);

    free(copy);
}

void benchmarkGraphSizes(void) {
    printf("\n===== GRAPH STRESS BENCHMARK =====\n");
    printf("Large-graph generator not yet implemented.\n");
}
