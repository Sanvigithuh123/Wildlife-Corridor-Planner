
#include <stdio.h>
#include <stdlib.h>
#include "priority.h"

// Merge two sorted sections
static void merge(
    HabitatPriority arr[],
    HabitatPriority temp[],
    int left,
    int mid,
    int right
) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {

        // Higher priority comes first
        if (arr[i].score >= arr[j].score)
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    for (int x = left; x <= right; x++)
        arr[x] = temp[x];
}

// Recursive Merge Sort
static void mergeSortPriority(
    HabitatPriority arr[],
    HabitatPriority temp[],
    int left,
    int right
) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSortPriority(arr, temp, left, mid);
    mergeSortPriority(arr, temp, mid + 1, right);

    merge(arr, temp, left, mid, right);
}

void rankHabitats(
    const Habitat habitats[],
    int habitatCount
) {
    if (habitatCount <= 0) {
        printf("No habitats available.\n");
        return;
    }

    // Find maximum population
    int maxPopulation = 0;

    for (int i = 0; i < habitatCount; i++) {

        if (habitats[i].population < 0 ||
            habitats[i].threatLevel < 0 ||
            habitats[i].threatLevel > 10) {

            printf("Invalid habitat data.\n");
            return;
        }

        if (habitats[i].population > maxPopulation)
            maxPopulation = habitats[i].population;
    }

    HabitatPriority *priority = malloc(
        (size_t)habitatCount * sizeof(HabitatPriority)
    );

    HabitatPriority *temp = malloc(
        (size_t)habitatCount * sizeof(HabitatPriority)
    );

    if (priority == NULL || temp == NULL) {
        printf("Memory allocation failed.\n");
        free(priority);
        free(temp);
        return;
    }

    // Calculate priority score
    for (int i = 0; i < habitatCount; i++) {

        priority[i].habitat = habitats[i];

        double threatScore =
            (habitats[i].threatLevel / 10.0) * 100.0;

        double populationRisk = 0.0;

        if (maxPopulation > 0) {
            populationRisk =
                (1.0 -
                (double)habitats[i].population /
                maxPopulation) * 100.0;
        }

        priority[i].score =
            0.7 * threatScore +
            0.3 * populationRisk;
    }

    // Sort in descending priority
    mergeSortPriority(
        priority,
        temp,
        0,
        habitatCount - 1
    );

    printf("\n===== CONSERVATION PRIORITY RANKING =====\n");

    printf(
        "%-6s %-15s %-12s %-10s %-10s\n",
        "Rank", "Habitat", "Population",
        "Threat", "Priority"
    );

    for (int i = 0; i < habitatCount; i++) {

        printf(
            "%-6d %-15s %-12d %-10d %-10.2f\n",
            i + 1,
            priority[i].habitat.name,
            priority[i].habitat.population,
            priority[i].habitat.threatLevel,
            priority[i].score
        );
    }

    printf(
        "\nHighest Priority Habitat: %s\n",
        priority[0].habitat.name
    );

    free(priority);
    free(temp);
}
