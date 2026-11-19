
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "string_matching.h"

typedef int (*Matcher)(
    const char *,
    const char *,
    MatchResult *
);

void compareStringMatching(
    const char *text,
    const char *pattern
) {
    const char *names[] = {
        "Naive",
        "Rabin-Karp",
        "Finite Automaton",
        "KMP"
    };

    Matcher methods[] = {
        naiveMatch,
        rabinKarpMatch,
        finiteAutomatonMatch,
        kmpMatch
    };

    MatchResult result;

    printf("\n===== STRING MATCHING COMPARISON =====\n");
    printf("Text: %s\n", text);
    printf("Pattern: %s\n\n", pattern);

    for (int i = 0; i < 4; i++) {

        clock_t start = clock();

        int count = methods[i](
            text, pattern, &result
        );

        clock_t end = clock();

        double milliseconds =
            1000.0 * (end - start) / CLOCKS_PER_SEC;

        printf("%s\n", names[i]);

        if (count < 0) {
            printf("Algorithm failed.\n\n");
            continue;
        }

        printf("Matches: %d\n", count);

        printf("Positions: ");

        for (int j = 0; j < count; j++)
            printf("%d ", result.positions[j]);

        printf("\nTime: %.6f ms\n\n", milliseconds);
    }
}
