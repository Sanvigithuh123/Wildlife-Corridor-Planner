
#include <stdio.h>
#include "habitat.h"
#include "graph.h"
#include "data_loader.h"
#include "kruskal.h"

int main() {

    Habitat habitats[MAX_HABITATS];
    Corridor corridors[MAX_CORRIDORS];

    int habitatCount = loadHabitats(
        "data/habitats.csv",
        habitats,
        MAX_HABITATS
    );

    int corridorCount = loadCorridors(
        "data/corridors.csv",
        corridors,
        MAX_CORRIDORS
    );

    if (habitatCount <= 0 || corridorCount < 0) {
        printf("Failed to load project data.\n");
        return 1;
    }

    printf("\nData loaded successfully!\n");
    printf("Habitats: %d\n", habitatCount);
    printf("Corridors: %d\n", corridorCount);

    int choice;

    do {
        printf("\n===== WILDLIFE CORRIDOR PLANNER =====\n");
        printf("1. Display Habitats\n");
        printf("2. Display Corridors\n");
        printf("3. Find Minimum Cost Corridors (Kruskal)\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice) {

            case 1:
                displayHabitats(habitats, habitatCount);
                break;

            case 2:
                displayGraph(corridors, corridorCount);
                break;

            case 3:
                kruskalMST(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount
                );
                break;

            case 0:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
