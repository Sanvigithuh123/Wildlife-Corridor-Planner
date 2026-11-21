
#include <stdio.h>
#include <string.h>

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
#include "priority.h"
#include "recommendations.h"

/* Minimal terminal theme */
#define RESET  "\033[0m"
#define CYAN   "\033[36m"
#define BOLD   "\033[1m"

static void printDivider(void) {
    printf("------------------------------------------------------------\n");
}

static void printHeader(void) {
    printf("\n");
    printDivider();

    printf(BOLD
           "           WILDLIFE CORRIDOR PLANNER\n"
           RESET);

    printf("        Conservation Decision Support System\n");

    printDivider();
}

static void printWelcome(
    int habitatCount,
    int corridorCount,
    int interventionCount,
    int activityCount
) {
    printHeader();

    printf("\n" CYAN "  DATA LOADED SUCCESSFULLY\n" RESET);
    printf("\n");

    printf("  Habitats       : %d\n", habitatCount);
    printf("  Corridors      : %d\n", corridorCount);
    printf("  Interventions  : %d\n", interventionCount);
    printf("  Activities     : %d\n", activityCount);

    printf("\n");
    printDivider();

    printf("  System ready for conservation analysis.\n");
}

static void printMenu(void) {
    printHeader();

    printf("\n" CYAN "  HABITAT MANAGEMENT\n" RESET);
    printf("  01  Display Habitats\n");
    printf("  02  Display Corridors\n");
    printf("  07  Sort Habitats (Merge Sort)\n");
    printf("  08  Sort Habitats (Quick Sort)\n");
    printf("  09  Search Habitat (Binary Search)\n");
    printf("  21  Conservation Priority Ranking\n");

    printf("\n" CYAN "  CORRIDOR AND ROUTE PLANNING\n" RESET);
    printf("  03  Minimum Cost Corridors (Kruskal)\n");
    printf("  04  Minimum Cost Corridors (Prim)\n");
    printf("  05  Shortest Wildlife Route (Dijkstra)\n");
    printf("  11  Shortest Wildlife Route (Bellman-Ford)\n");
    printf("  13  Corridor Capacity (Ford-Fulkerson)\n");

    printf("\n" CYAN "  CONSERVATION RESOURCE PLANNING\n" RESET);
    printf("  06  Budget Optimization (0/1 Knapsack)\n");
    printf("  12  Budget Optimization (Branch-and-Bound)\n");
    printf("  10  Conservation Activity Selection\n");
    printf("  14  Conservation Team Assignment\n");

    printf("\n" CYAN "  SCENARIOS AND PERFORMANCE\n" RESET);
    printf("  15  Normal Conservation Scenario\n");
    printf("  16  Corridor Constraint Shock Scenario\n");
    printf("  17  Sorting Benchmark\n");
    printf("  20  Large Data Stress Benchmark\n");

    printf("\n" CYAN "  REPORTS AND EXPORTS\n" RESET);
    printf("  18  Export Network to CSV\n");
    printf("  19  Export Network to JSON\n");
    printf("  22  Export Conservation Recommendations\n");

    printf("\n");
    printDivider();
    printf("  00  Exit\n");
    printDivider();

    printf(CYAN "\n  Select an option: " RESET);
    fflush(stdout);
}

int main(void) {

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

    printWelcome(
        habitatCount,
        corridorCount,
        interventionCount,
        activityCount
    );

    int choice;

    do {
        printMenu();

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice) {

            case 1:
                displayHabitats(
                    habitats,
                    habitatCount
                );
                break;

            case 2:
                displayGraph(
                    corridors,
                    corridorCount
                );
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
                    (size_t)habitatCount * sizeof(Habitat)
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

                mergeSort(
                    sorted,
                    habitatCount,
                    sortBy
                );

                printf("\n===== MERGE SORT RESULT =====\n");

                displayHabitats(
                    sorted,
                    habitatCount
                );

                break;
            }

            case 8: {
                Habitat sorted[MAX_HABITATS];
                int sortBy;

                memcpy(
                    sorted,
                    habitats,
                    (size_t)habitatCount * sizeof(Habitat)
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

                quickSort(
                    sorted,
                    habitatCount,
                    sortBy
                );

                printf("\n===== QUICK SORT RESULT =====\n");

                displayHabitats(
                    sorted,
                    habitatCount
                );

                break;
            }

            case 9: {
                Habitat sorted[MAX_HABITATS];
                int targetId;
                int method;

                memcpy(
                    sorted,
                    habitats,
                    (size_t)habitatCount * sizeof(Habitat)
                );

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
                        sorted,
                        habitatCount,
                        targetId
                    );
                }
                else {
                    index = binarySearchRecursive(
                        sorted,
                        habitatCount,
                        targetId
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

            case 10:
                activitySelection(
                    activities,
                    activityCount
                );
                break;

            case 11: {
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

            case 12: {
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

            case 13: {
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

            case 14: {
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

            case 15:
                runNormalScenario(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount
                );
                break;

            case 16:
                runShockScenario(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount
                );
                break;

            case 17:
                benchmarkSorting(
                    habitats,
                    habitatCount
                );
                break;

            case 18:
                exportCSV(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount
                );
                break;

            case 19:
                exportJSON(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount
                );
                break;

            case 20:
                benchmarkGraphSizes();
                break;

            case 21:
                rankHabitats(
                    habitats,
                    habitatCount
                );
                break;

            case 22: {
                int budget;

                printf("Enter conservation budget (lakh): ");

                if (scanf("%d", &budget) != 1) {
                    puts("Invalid budget.");
                    break;
                }

                exportRecommendations(
                    habitats,
                    habitatCount,
                    corridors,
                    corridorCount,
                    interventions,
                    interventionCount,
                    budget
                );

                break;
            }

            case 0:
                printf("\nExiting Wildlife Corridor Planner.\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
