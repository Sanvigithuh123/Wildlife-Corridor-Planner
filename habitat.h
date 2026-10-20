#ifndef HABITAT_H
#define HABITAT_H

typedef struct {
    int id;
    char name[50];
    int population;
    int threatLevel;
}Habitat;

typedef struct{
    int source;
    int destination;
    int cost;
    int capacity;
    int available;
}Corridor;

#endif