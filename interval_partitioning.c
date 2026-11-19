
#include <stdio.h>
#include "scheduling.h"

void intervalPartitioning(
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

    // Sort by increasing start time
    for (int i = 1; i < n; i++) {
        Activity current = sorted[i];
        int j = i - 1;

        while (j >= 0 &&
               sorted[j].start > current.start) {
            sorted[j + 1] = sorted[j];
            j--;
        }

        sorted[j + 1] = current;
    }

    int teamFinish[MAX_ACTIVITIES];
    int teamCount = 0;

    printf("\n===== INTERVAL PARTITIONING =====\n");

    for (int i = 0; i < n; i++) {

        int assignedTeam = -1;

        // Find an available team
        for (int j = 0; j < teamCount; j++) {

            if (teamFinish[j] <= sorted[i].start) {
                assignedTeam = j;
                break;
            }
        }

        // No team available: create a new one
        if (assignedTeam == -1) {
            assignedTeam = teamCount;
            teamCount++;
        }

        teamFinish[assignedTeam] = sorted[i].finish;

        printf(
            "%s | %d - %d | Team %d\n",
            sorted[i].name,
            sorted[i].start,
            sorted[i].finish,
            assignedTeam + 1
        );
    }

    printf("\nMinimum Teams Required: %d\n",
           teamCount);
}
