
#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

// Extract the sorting key
static int getKey(Habitat h, int sortBy) {

    if (sortBy == SORT_BY_POPULATION)
        return h.population;

    if (sortBy == SORT_BY_THREAT)
        return h.threatLevel;

    return h.id;
}

// Merge two sorted halves
static void merge(
    Habitat arr[],
    Habitat temp[],
    int left,
    int mid,
    int right,
    int sortBy
) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {

        if (getKey(arr[i], sortBy) <=
            getKey(arr[j], sortBy)) {

            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    for (int x = left; x <= right; x++)
        arr[x] = temp[x];
}

// Recursive divide-and-conquer
static void mergeSortRecursive(
    Habitat arr[],
    Habitat temp[],
    int left,
    int right,
    int sortBy
) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSortRecursive(
        arr, temp, left, mid, sortBy
    );

    mergeSortRecursive(
        arr, temp, mid + 1, right, sortBy
    );

    merge(
        arr, temp, left, mid, right, sortBy
    );
}

void mergeSort(
    Habitat arr[],
    int n,
    int sortBy
) {
    if (n <= 1)
        return;

    Habitat *temp = malloc(
        (size_t)n * sizeof(Habitat)
    );

    if (temp == NULL) {
        printf("Merge Sort: memory allocation failed.\n");
        return;
    }

    mergeSortRecursive(
        arr, temp, 0, n - 1, sortBy
    );

    free(temp);
}
