#include <stdio.h>
#include "habitat.h"
#include "graph.h"
#include "data_loader.h"
#include "kruskal.h"
#include "prim.h"
#include "dijkstra.h"
#include "knapsack.h"
#include "sorting.h"
#include "binary_search.h"
#include "scheduling.h"
#include "bellman_ford.h"
#include "branch_bound.h"
#include "ford_fulkerson.h"
#include "bipartite_matching.h"
#include "scenario.h"
#include "benchmark.h"
#include "export.h"
#include<string.h>

static int loadEventSequence(
    const char *filename,
    char text[],
    int maxLength
) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    int count = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {

        if (ch == '\n' || ch == '\r' ||
            ch == ' ' || ch == '\t')
            continue;

        if (count >= maxLength - 1) {
            fclose(fp);
            return -1;
        }

        text[count++] = (char)ch;
    }

    if (ferror(fp)) {
        fclose(fp);
        return -1;
    }

    text[count] = '\0';
    fclose(fp);

    return count;
}


int main() {

    Habitat habitats[MAX_HABITATS];
    Corridor corridors[MAX_CORRIDORS];
    Intervention interventions[MAX_INTERVENTIONS];
    Activity activities[MAX_ACTIVITIES];

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
    int activityCount = loadActivities(
        "data/activities.csv",
        activities,
        MAX_ACTIVITIES
    );

    if (habitatCount <= 0 || corridorCount < 0) {
        printf("Failed to load project data.\n");
        return 1;
    }
    if (interventionCount < 0) {
        printf("Failed to load interventions.\n");
        return 1;
    }
    if (activityCount < 0) {
        printf("Failed to load activities.\n");
        return 1;
    }

    printf("\nData loaded successfully!\n");
    printf("Habitats: %d\n", habitatCount);
    printf("Corridors: %d\n", corridorCount);
    printf("Interventions loaded: %d\n", interventionCount);
    printf("Activities loaded: %d\n", activityCount);

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
        printf("11. Conservation Activity Selection\n");
        printf("15. Shortest Route (Bellman-Ford)\n");
        printf("19. Budget Optimization (Branch-and-Bound)\n");
        printf("20. Analyze Corridor Capacity (Ford-Fulkerson)\n");
        printf("21. Assign Conservation Teams (Bipartite Matching)\n");
        printf("23. Normal Conservation Scenario\n");
        printf("24. Corridor Constraint Shock Scenario\n");
        printf("25. Sorting Benchmark\n");
        printf("26. Export Network to CSV\n");
        printf("27. Export Network to JSON\n");
        printf("28. Large Data Stress Benchmark\n");
                
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

        case 7: {
            Habitat sorted[MAX_HABITATS];
            int sortBy;

            memcpy(
                sorted,
                habitats,
                habitatCount * sizeof(Habitat)
            );

            printf("\nSort habitats by:\n");
            printf("1. ID\n");
            printf("2. Population\n");
            printf("3. Threat Level\n");
            printf("Enter choice: ");

            if (scanf("%d", &sortBy) != 1 ||
                sortBy < 1 || sortBy > 3) {
                printf("Invalid sorting choice.\n");
                break;
            }

            mergeSort(sorted, habitatCount, sortBy);

            printf("\n===== MERGE SORT RESULT =====\n");
            displayHabitats(sorted, habitatCount);

            break;
        }


        case 8: {
            Habitat sorted[MAX_HABITATS];
            int sortBy;

            memcpy(
                sorted,
                habitats,
                habitatCount * sizeof(Habitat)
            );

            printf("\nSort habitats by:\n");
            printf("1. ID\n");
            printf("2. Population\n");
            printf("3. Threat Level\n");
            printf("Enter choice: ");

            if (scanf("%d", &sortBy) != 1 ||
                sortBy < 1 || sortBy > 3) {
                printf("Invalid sorting choice.\n");
                break;
            }

            quickSort(sorted, habitatCount, sortBy);

            printf("\n===== QUICK SORT RESULT =====\n");
            displayHabitats(sorted, habitatCount);

            break;
        }


        case 9: {
            Habitat sorted[MAX_HABITATS];
            int targetId;
            int method;

            memcpy(
                sorted,
                habitats,
                habitatCount * sizeof(Habitat)
            );

            // Binary search requires sorted IDs
            mergeSort(
                sorted,
                habitatCount,
                SORT_BY_ID
            );

            printf("Enter habitat ID to search: ");

            if (scanf("%d", &targetId) != 1) {
                printf("Invalid habitat ID.\n");
                break;
            }

            printf("1. Iterative Binary Search\n");
            printf("2. Recursive Binary Search\n");
            printf("Enter method: ");

            if (scanf("%d", &method) != 1 ||
                (method != 1 && method != 2)) {
                printf("Invalid search method.\n");
                break;
            }

            int index;

            if (method == 1) {
                index = binarySearchIterative(
                    sorted, habitatCount, targetId
                );
            }
            else {
                index = binarySearchRecursive(
                    sorted, habitatCount, targetId
                );
            }

            if (index == -1) {
                printf("Habitat not found.\n");
            }
            else {
                printf("\nHabitat Found!\n");
                printf("ID: %d\n", sorted[index].id);
                printf("Name: %s\n", sorted[index].name);
                printf("Population: %d\n",
                    sorted[index].population);
                printf("Threat Level: %d\n",
                    sorted[index].threatLevel);
            }

            break;
        }


        

            case 11:
                activitySelection(
                    activities,
                    activityCount
                );
                break;

            


            case 15: {
                int sourceId, destinationId;

                printf("Enter source habitat ID: ");

                if (scanf("%d", &sourceId) != 1) {
                    printf("Invalid source ID.\n");
                    break;
                }

                printf("Enter destination habitat ID: ");

                if (scanf("%d", &destinationId) != 1) {
                    printf("Invalid destination ID.\n");
                    break;
                }

                bellmanFord(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount,
                    sourceId,
                    destinationId
                );

                break;
            }



            


            case 19: {
                int budget;
                int method;

                printf("Enter conservation budget (lakh): ");

                if (scanf("%d", &budget) != 1) {
                    printf("Invalid budget.\n");
                    break;
                }

                printf("\nSelect Branch-and-Bound Method:\n");
                printf("1. FIFO\n");
                printf("2. LIFO\n");
                printf("3. Best-Bound\n");
                printf("Enter choice: ");

                if (scanf("%d", &method) != 1) {
                    printf("Invalid method.\n");
                    break;
                }

                branchBoundKnapsack(
                    interventions,
                    interventionCount,
                    budget,
                    method
                );

                break;
            }


    case 20: {
        int sourceId, destinationId;

        printf("Enter source habitat ID: ");
        if (scanf("%d", &sourceId) != 1) {
            printf("Invalid source ID.\n");
            break;
        }

        printf("Enter destination habitat ID: ");
        if (scanf("%d", &destinationId) != 1) {
            printf("Invalid destination ID.\n");
            break;
        }

        fordFulkerson(
            habitats,
            habitatCount,
            corridors,
            corridorCount,
            sourceId,
            destinationId
        );

        break;
    }


    case 21: {
        int compatibility[MAX_TEAMS][MAX_TASKS] = {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {1, 0, 0, 1}
        };

        maximumBipartiteMatching(
            3,
            4,
            compatibility
        );

        break;
    }


        

        case 23:
            runNormalScenario(
                habitats,
                habitatCount,
                corridors,
                corridorCount
            );
            break;

        case 24:
            runShockScenario(
                habitats,
                habitatCount,
                corridors,
                corridorCount
            );
            break;

        case 25:
            benchmarkSorting(
                habitats,
                habitatCount
            );
            break;

        case 26:
            exportCSV(
                habitats,
                habitatCount,
                corridors,
                corridorCount
            );
            break;

        case 27:
            exportJSON(
                habitats,
                habitatCount,
                corridors,
                corridorCount
            );
            break;

            case 28:
                benchmarkGraphSizes();
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
