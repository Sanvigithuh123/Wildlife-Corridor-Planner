
#include "binary_search.h"

// Iterative binary search
int binarySearchIterative(
    const Habitat arr[],
    int n,
    int targetId
) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {

        int mid = low + (high - low) / 2;

        if (arr[mid].id == targetId)
            return mid;

        if (arr[mid].id < targetId)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

// Recursive helper
static int recursiveHelper(
    const Habitat arr[],
    int low,
    int high,
    int targetId
) {
    if (low > high)
        return -1;

    int mid = low + (high - low) / 2;

    if (arr[mid].id == targetId)
        return mid;

    if (arr[mid].id < targetId) {
        return recursiveHelper(
            arr, mid + 1, high, targetId
        );
    }

    return recursiveHelper(
        arr, low, mid - 1, targetId
    );
}

// Recursive binary search
int binarySearchRecursive(
    const Habitat arr[],
    int n,
    int targetId
) {
    return recursiveHelper(
        arr, 0, n - 1, targetId
    );
}
