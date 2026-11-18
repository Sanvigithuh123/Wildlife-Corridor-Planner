
#include <stdio.h>
#include <limits.h>
#include "max_subarray.h"

// Find maximum subarray crossing the middle
static long long maxCrossingSum(
    int arr[],
    int low,
    int mid,
    int high
) {
    long long sum = 0;
    long long leftSum = LLONG_MIN;

    for (int i = mid; i >= low; i--) {
        sum += arr[i];

        if (sum > leftSum)
            leftSum = sum;
    }

    sum = 0;
    long long rightSum = LLONG_MIN;

    for (int i = mid + 1; i <= high; i++) {
        sum += arr[i];

        if (sum > rightSum)
            rightSum = sum;
    }

    return leftSum + rightSum;
}

// Recursive divide-and-conquer function
static long long maxSubarrayRecursive(
    int arr[],
    int low,
    int high
) {
    if (low == high)
        return arr[low];

    int mid = low + (high - low) / 2;

    long long left = maxSubarrayRecursive(
        arr, low, mid
    );

    long long right = maxSubarrayRecursive(
        arr, mid + 1, high
    );

    long long crossing = maxCrossingSum(
        arr, low, mid, high
    );

    long long maximum = left;

    if (right > maximum)
        maximum = right;

    if (crossing > maximum)
        maximum = crossing;

    return maximum;
}

void maximumSubarray(int arr[], int n) {

    if (n <= 0) {
        printf("Invalid array size.\n");
        return;
    }

    long long result = maxSubarrayRecursive(
        arr, 0, n - 1
    );

    printf("\n===== MAXIMUM SUBARRAY =====\n");
    printf("Maximum cumulative threat increase: %lld\n",
           result);
}
