
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"

void longestCommonSubsequence(
    const char *seq1,
    const char *seq2
) {
    int m = (int)strlen(seq1);
    int n = (int)strlen(seq2);

    size_t cols = (size_t)n + 1;
    size_t rows = (size_t)m + 1;

    // Allocate a flattened DP table
    int *dp = calloc(rows * cols, sizeof(int));

    if (dp == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Build DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {

            size_t current = (size_t)i * cols + j;

            if (seq1[i - 1] == seq2[j - 1]) {
                dp[current] =
                    1 + dp[(size_t)(i - 1) * cols + j - 1];
            }
            else {
                int up = dp[(size_t)(i - 1) * cols + j];
                int left = dp[(size_t)i * cols + j - 1];

                dp[current] = up > left ? up : left;
            }
        }
    }

    int length = dp[(size_t)m * cols + n];

    char *result = malloc((size_t)length + 1);

    if (result == NULL) {
        printf("Memory allocation failed.\n");
        free(dp);
        return;
    }

    result[length] = '\0';

    // Reconstruct the LCS
    int i = m;
    int j = n;
    int k = length;

    while (i > 0 && j > 0) {

        if (seq1[i - 1] == seq2[j - 1]) {
            result[--k] = seq1[i - 1];
            i--;
            j--;
        }
        else if (
            dp[(size_t)(i - 1) * cols + j] >=
            dp[(size_t)i * cols + j - 1]
        ) {
            i--;
        }
        else {
            j--;
        }
    }

    printf("\n===== LONGEST COMMON SUBSEQUENCE =====\n");
    printf("Sequence 1: %s\n", seq1);
    printf("Sequence 2: %s\n", seq2);
    printf("LCS Length: %d\n", length);
    printf("LCS: %s\n", result);

    free(result);
    free(dp);
}
