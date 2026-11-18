
#include <stdio.h>
#include "celebrity.h"

// Recursively eliminate one candidate
static int findCandidate(
    int matrix[MAX_CENTERS][MAX_CENTERS],
    int n
) {
    if (n == 1)
        return 0;

    int candidate = findCandidate(matrix, n - 1);
    int other = n - 1;

    if (matrix[candidate][other] == 1)
        return other;

    return candidate;
}

int findCelebrity(
    int matrix[MAX_CENTERS][MAX_CENTERS],
    int n
) {
    if (n <= 0 || n > MAX_CENTERS)
        return -1;

    int candidate = findCandidate(matrix, n);

    // Verify the candidate
    for (int i = 0; i < n; i++) {

        if (i == candidate)
            continue;

        if (matrix[candidate][i] != 0 ||
            matrix[i][candidate] != 1) {

            return -1;
        }
    }

    return candidate;
}
