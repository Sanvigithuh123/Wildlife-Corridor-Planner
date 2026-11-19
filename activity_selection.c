
#include <stdio.h>
#include "scheduling.h"

void activitySelection(
    const Activity activities[],
    int n
) {
    if (n < 0 || n > MAX_ACTIVITIES) {
        printf("Invalid number of activities.\n");
        return;
    }

    if (n == 0) {
        printf("No activities available.\n");
        return;
    }

    Activity sorted[MAX_ACTIVITIES];

    for (int i = 0; i < n; i++)
        sorted[i] = activities[i];

    // Sort by increasing finish time
    for (int i = 1; i < n; i++) {
        Activity current = sorted[i];
        int j = i - 1;

        while (j >= 0 &&
               sorted[j].finish > current.finish) {
            sorted[j + 1] = sorted[j];
            j--;
        }

        sorted[j + 1] = current;
    }

    printf("\n===== ACTIVITY SELECTION =====\n");
    printf("Selected Conservation Activities:\n");

    int lastFinish = sorted[0].finish;
    int selected = 1;

    printf(
        "%s | %d - %d\n",
        sorted[0].name,
        sorted[0].start,
        sorted[0].finish
    );

    for (int i = 1; i < n; i++) {

        if (sorted[i].start >= lastFinish) {

            printf(
                "%s | %d - %d\n",
                sorted[i].name,
                sorted[i].start,
                sorted[i].finish
            );

            lastFinish = sorted[i].finish;
            selected++;
        }
    }

    printf("\nMaximum Non-Overlapping Activities: %d\n",
           selected);
}
