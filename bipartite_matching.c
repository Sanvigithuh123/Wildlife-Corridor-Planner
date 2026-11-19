
#include <stdio.h>
#include "bipartite_matching.h"

static int tryAssign(
    int team,
    int taskCount,
    int compatibility[MAX_TEAMS][MAX_TASKS],
    int visited[],
    int assignedTeam[]
) {
    for (int task = 0; task < taskCount; task++) {

        if (!compatibility[team][task] ||
            visited[task])
            continue;

        visited[task] = 1;

        if (assignedTeam[task] == -1 ||
            tryAssign(
                assignedTeam[task],
                taskCount,
                compatibility,
                visited,
                assignedTeam
            )) {

            assignedTeam[task] = team;
            return 1;
        }
    }

    return 0;
}

void maximumBipartiteMatching(
    int teamCount,
    int taskCount,
    int compatibility[MAX_TEAMS][MAX_TASKS]
) {
    if (teamCount < 0 || teamCount > MAX_TEAMS ||
        taskCount < 0 || taskCount > MAX_TASKS) {
        printf("Invalid team or task count.\n");
        return;
    }

    int assignedTeam[MAX_TASKS];

    for (int i = 0; i < taskCount; i++)
        assignedTeam[i] = -1;

    int matches = 0;

    for (int team = 0; team < teamCount; team++) {

        int visited[MAX_TASKS] = {0};

        if (tryAssign(
            team,
            taskCount,
            compatibility,
            visited,
            assignedTeam
        )) {
            matches++;
        }
    }

    printf("\n===== MAXIMUM BIPARTITE MATCHING =====\n");

    for (int task = 0; task < taskCount; task++) {
        if (assignedTeam[task] != -1) {
            printf(
                "Team %d -> Task %d\n",
                assignedTeam[task] + 1,
                task + 1
            );
        }
    }

    printf("\nMaximum Teams Assigned: %d\n", matches);
    printf("Unassigned Teams: %d\n",
           teamCount - matches);
}
