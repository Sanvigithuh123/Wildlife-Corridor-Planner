
#include <stdio.h>
#include "scheduling.h"

int loadActivities(
    const char *filename,
    Activity activities[],
    int maxActivities
) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Cannot open %s\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    // Skip CSV header
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp)) {

        if (count >= maxActivities) {
            printf("Too many activities.\n");
            fclose(fp);
            return -1;
        }

        Activity a;

        int fields = sscanf(
            line,
            "%d,%49[^,],%d,%d",
            &a.id,
            a.name,
            &a.start,
            &a.finish
        );

        if (fields != 4 ||
            a.id < 0 ||
            a.start < 0 ||
            a.finish <= a.start) {

            printf("Invalid activity record.\n");
            fclose(fp);
            return -1;
        }

        for (int i = 0; i < count; i++) {
            if (activities[i].id == a.id) {
                printf("Duplicate activity ID.\n");
                fclose(fp);
                return -1;
            }
        }

        activities[count++] = a;
    }

    fclose(fp);
    return count;
}
