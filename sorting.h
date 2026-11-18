
#ifndef SORTING_H
#define SORTING_H

#include "habitat.h"

#define SORT_BY_ID 1
#define SORT_BY_POPULATION 2
#define SORT_BY_THREAT 3

void mergeSort(
    Habitat arr[],
    int n,
    int sortBy
);

void quickSort(
    Habitat arr[],
    int n,
    int sortBy
);

#endif
