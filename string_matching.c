
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "string_matching.h"

static void resetResult(MatchResult *r) {
    r->count = 0;
}

static void addMatch(MatchResult *r, int pos) {
    if (r->count < MAX_TEXT)
        r->positions[r->count++] = pos;
}

// 1. NAIVE STRING MATCHING

int naiveMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
) {
    resetResult(result);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0 || m > n)
        return 0;

    for (int i = 0; i <= n - m; i++) {

        int j = 0;

        while (j < m && text[i + j] == pattern[j])
            j++;

        if (j == m)
            addMatch(result, i);
    }

    return result->count;
}

// 2. RABIN-KARP

int rabinKarpMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
) {
    resetResult(result);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0 || m > n)
        return 0;

    const int d = 256;
    const int q = 101;

    int patternHash = 0;
    int windowHash = 0;
    int h = 1;

    for (int i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (int i = 0; i < m; i++) {
        patternHash =
            (d * patternHash + (unsigned char)pattern[i]) % q;

        windowHash =
            (d * windowHash + (unsigned char)text[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {

        if (patternHash == windowHash) {

            int j = 0;

            while (j < m &&
                   text[i + j] == pattern[j + 0])
                j++;

            if (j == m)
                addMatch(result, i);
        }

        if (i < n - m) {
            windowHash =
                (d * (windowHash -
                    (unsigned char)text[i] * h) +
                    (unsigned char)text[i + m]) % q;

            if (windowHash < 0)
                windowHash += q;
        }
    }

    return result->count;
}

// 3. KMP PREFIX TABLE

static void buildLPS(
    const char *pattern,
    int m,
    int lps[]
) {
    lps[0] = 0;

    int len = 0;
    int i = 1;

    while (i < m) {

        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        }
        else if (len != 0) {
            len = lps[len - 1];
        }
        else {
            lps[i++] = 0;
        }
    }
}

// 4. KMP MATCHING

int kmpMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
) {
    resetResult(result);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0 || m > n)
        return 0;

    int *lps = malloc((size_t)m * sizeof(int));

    if (lps == NULL)
        return -1;

    buildLPS(pattern, m, lps);

    int i = 0;
    int j = 0;

    while (i < n) {

        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            addMatch(result, i - j);
            j = lps[j - 1];
        }
        else if (i < n && text[i] != pattern[j]) {

            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    free(lps);
    return result->count;
}

// 5. FINITE AUTOMATON MATCHING

int finiteAutomatonMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
) {
    resetResult(result);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0 || m > n)
        return 0;

    int *lps = malloc((size_t)m * sizeof(int));

    int (*transition)[256] = malloc(
        ((size_t)m + 1) * sizeof(*transition)
    );

    if (lps == NULL || transition == NULL) {
        free(lps);
        free(transition);
        return -1;
    }

    buildLPS(pattern, m, lps);

    // Transition table for state 0
    for (int c = 0; c < 256; c++)
        transition[0][c] = 0;

    transition[0][(unsigned char)pattern[0]] = 1;

    // Build remaining states
    for (int state = 1; state <= m; state++) {

        int fallback = lps[state - 1];

        for (int c = 0; c < 256; c++) {

            if (state < m &&
                c == (unsigned char)pattern[state]) {
                transition[state][c] = state + 1;
            }
            else {
                transition[state][c] =
                    transition[fallback][c];
            }
        }
    }

    int state = 0;

    for (int i = 0; i < n; i++) {

        state = transition[state][(unsigned char)text[i]];

        if (state == m)
            addMatch(result, i - m + 1);
    }

    free(lps);
    free(transition);

    return result->count;
}
