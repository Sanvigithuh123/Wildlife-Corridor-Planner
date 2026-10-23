
#include <stdio.h>
#include "graph.h"

void displayHabitats(Habitat habitats[], int count) {

    printf("\n===== HABITAT RECORDS =====\n");

    for (int i = 0; i < count; i++) {
        printf(
            "ID: %d | Name: %s | Population: %d | Threat: %d\n",
            habitats[i].id,
            habitats[i].name,
            habitats[i].population,
            habitats[i].threatLevel
        );
    }
}

void displayGraph(Corridor edges[], int edgeCount) {

    printf("\n===== AVAILABLE CORRIDORS =====\n");

    for (int i = 0; i < edgeCount; i++) {

        if (edges[i].available == 1) {
            printf(
                "%d -> %d | Cost: %d | Capacity: %d\n",
                edges[i].source,
                edges[i].destination,
                edges[i].cost,
                edges[i].capacity
            );
        }
    }
}
