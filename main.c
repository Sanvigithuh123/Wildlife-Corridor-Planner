#include <stdio.h>
#include "habitat.h"
#include "graph.h"
#include "data_loader.h"
#include "kruskal.h"
#include "prim.h"
#include "dijkstra.h"
#include "knapsack.h"
#include <string.h>
#include "sorting.h"
#include "binary_search.h"

int main() {

    Habitat habitats[MAX_HABITATS];
    Corridor corridors[MAX_CORRIDORS];
    Intervention interventions[MAX_INTERVENTIONS];

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

    int interventionCount = loadInterventions(
        "data/interventions.csv",
        interventions,
        MAX_INTERVENTIONS
    );

    if (habitatCount <= 0 || corridorCount < 0) {
        printf("Failed to load project data.\n");
        return 1;
    }
    if (interventionCount < 0) {
        printf("Failed to load interventions.\n");
        return 1;
    }

    printf("\nData loaded successfully!\n");
    printf("Habitats: %d\n", habitatCount);
    printf("Corridors: %d\n", corridorCount);
    printf("Interventions loaded: %d\n", interventionCount);

    int choice;

    do {
        printf("\n===== WILDLIFE CORRIDOR PLANNER =====\n");
        printf("1. Display Habitats\n");
        printf("2. Display Corridors\n");
        printf("3. Find Minimum Cost Corridors (Kruskal)\n");
        printf("4. Find Minimum Cost Corridors (Prim)\n");
        printf("5. Find Shortest Wildlife Route (Dijkstra)\n");
        printf("6. Optimize Conservation Budget (0/1 Knapsack)\n");
        printf("7. Sort Habitats (Merge Sort)\n");
        printf("8. Sort Habitats (Quick Sort)\n");
        printf("9. Search Habitat (Binary Search)\n");
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
            
            case 4:
                primMST(
                habitats,
                habitatCount,
                corridors,
                corridorCount
            );
            break;

            case 5: {
                int sourceId, destinationId;

                printf("Enter source habitat ID: ");
                if (scanf("%d", &sourceId) != 1) {
                    printf("Invalid source ID.\n");
                    return 1;
                }

                printf("Enter destination habitat ID: ");
                if (scanf("%d", &destinationId) != 1) {
                    printf("Invalid destination ID.\n");
                    return 1;
                }

                dijkstra(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount,
                    sourceId,
                    destinationId
                );

                break;
            }

        case 6: {
            int budget;

            printf("Enter conservation budget (in lakh): ");

            if (scanf("%d", &budget) != 1) {
                printf("Invalid budget input.\n");
                return 1;
            }

            optimizeBudget(
                interventions,
                interventionCount,
                budget
            );

            break;
        }


            case 0:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
