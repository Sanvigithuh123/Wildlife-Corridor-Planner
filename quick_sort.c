
#include "sorting.h"

static int getKey(Habitat h, int sortBy) {

    if (sortBy == SORT_BY_POPULATION)
        return h.population;

    if (sortBy == SORT_BY_THREAT)
        return h.threatLevel;

    return h.id;
}

static void swap(Habitat *a, Habitat *b) {

    Habitat temp = *a;
    *a = *b;
    *b = temp;
}

// Lomuto partition
static int partition(
    Habitat arr[],
    int low,
    int high,
    int sortBy
) {
    int pivot = getKey(arr[high], sortBy);
    int i = low - 1;

    for (int j = low; j < high; j++) {

        if (getKey(arr[j], sortBy) <= pivot) {

            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

static void quickSortRecursive(
    Habitat arr[],
    int low,
    int high,
    int sortBy
) {
    if (low < high) {

        int pivotIndex = partition(
            arr, low, high, sortBy
        );

        quickSortRecursive(
            arr, low, pivotIndex - 1, sortBy
        );

        quickSortRecursive(
            arr, pivotIndex + 1, high, sortBy
        );
    }
}

void quickSort(
    Habitat arr[],
    int n,
    int sortBy
) {
    if (n <= 1)
        return;

    quickSortRecursive(
        arr, 0, n - 1, sortBy
    );
}
