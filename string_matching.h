
#ifndef STRING_MATCHING_H
#define STRING_MATCHING_H

#define MAX_TEXT 100000
#define MAX_PATTERN 1000

typedef struct {
    int positions[MAX_TEXT];
    int count;
} MatchResult;

int naiveMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
);

int rabinKarpMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
);

int finiteAutomatonMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
);

int kmpMatch(
    const char *text,
    const char *pattern,
    MatchResult *result
);

void compareStringMatching(
    const char *text,
    const char *pattern
);

#endif
