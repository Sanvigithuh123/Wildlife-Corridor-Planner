
#ifndef SCHEDULING_H
#define SCHEDULING_H

#define MAX_ACTIVITIES 100

typedef struct {
    int id;
    char name[50];
    int start;
    int finish;
} Activity;

int loadActivities(
    const char *filename,
    Activity activities[],
    int maxActivities
);

void activitySelection(
    const Activity activities[],
    int n
);

void intervalPartitioning(
    const Activity activities[],
    int n
);

#endif
