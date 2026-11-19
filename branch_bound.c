
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "branch_bound.h"

#define MAX_BB_ITEMS 20

typedef struct {
    Intervention item;
    int originalIndex;
} BBItem;

typedef struct {
    int level;
    int cost;
    long long benefit;
    double bound;
    uint32_t mask;
} BBNode;

typedef struct {
    BBNode *data;
    size_t size;
    size_t capacity;
    size_t head;
    int method;
} Frontier;

static int compareRatio(
    const BBItem *a,
    const BBItem *b
) {
    long long left =
        (long long)a->item.benefit * b->item.cost;

    long long right =
        (long long)b->item.benefit * a->item.cost;

    if (left > right) return -1;
    if (left < right) return 1;
    return 0;
}

static void sortByRatio(BBItem arr[], int n) {
    for (int i = 1; i < n; i++) {
        BBItem current = arr[i];
        int j = i - 1;

        while (j >= 0 &&
               compareRatio(&current, &arr[j]) < 0) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = current;
    }
}

static double calculateBound(
    const BBNode *node,
    const BBItem items[],
    int n,
    int budget
) {
    if (node->cost > budget)
        return -1.0;

    double bound = (double)node->benefit;
    int remaining = budget - node->cost;

    for (int i = node->level; i < n; i++) {

        int cost = items[i].item.cost;
        int benefit = items[i].item.benefit;

        if (cost <= remaining) {
            remaining -= cost;
            bound += benefit;
        }
        else {
            bound +=
                (double)remaining * benefit / cost;
            break;
        }
    }

    return bound;
}

static int frontierPush(
    Frontier *f,
    BBNode node
) {
    if (f->size == f->capacity) {
        size_t newCapacity =
            f->capacity == 0 ? 64 : f->capacity * 2;

        BBNode *temp = realloc(
            f->data,
            newCapacity * sizeof(BBNode)
        );

        if (temp == NULL)
            return 0;

        f->data = temp;
        f->capacity = newCapacity;
    }

    if (f->method == BB_BEST_BOUND) {
        size_t i = f->size++;

        while (i > 0) {
            size_t parent = (i - 1) / 2;

            if (f->data[parent].bound >= node.bound)
                break;

            f->data[i] = f->data[parent];
            i = parent;
        }

        f->data[i] = node;
    }
    else {
        f->data[f->size++] = node;
    }

    return 1;
}

static BBNode frontierPop(Frontier *f) {
    if (f->method == BB_FIFO) {
        return f->data[f->head++];
    }

    if (f->method == BB_LIFO) {
        return f->data[--f->size];
    }

    BBNode result = f->data[0];
    BBNode last = f->data[--f->size];

    if (f->size > 0) {
        size_t i = 0;

        while (2 * i + 1 < f->size) {
            size_t child = 2 * i + 1;

            if (child + 1 < f->size &&
                f->data[child + 1].bound >
                f->data[child].bound) {
                child++;
            }

            if (last.bound >= f->data[child].bound)
                break;

            f->data[i] = f->data[child];
            i = child;
        }

        f->data[i] = last;
    }

    return result;
}

static int frontierEmpty(const Frontier *f) {
    if (f->method == BB_FIFO)
        return f->head >= f->size;

    return f->size == 0;
}

void branchBoundKnapsack(
    const Intervention items[],
    int n,
    int budget,
    int method
) {
    if (n < 0 || n > MAX_BB_ITEMS ||
        budget < 0 ||
        (method != BB_FIFO &&
         method != BB_LIFO &&
         method != BB_BEST_BOUND)) {
        printf("Invalid Branch-and-Bound input.\n");
        return;
    }

    BBItem sorted[MAX_BB_ITEMS];

    for (int i = 0; i < n; i++) {
        if (items[i].cost <= 0 ||
            items[i].benefit < 0) {
            printf("Invalid intervention cost or benefit.\n");
            return;
        }

        sorted[i].item = items[i];
        sorted[i].originalIndex = i;
    }

    sortByRatio(sorted, n);

    Frontier frontier = {
        .data = NULL,
        .size = 0,
        .capacity = 0,
        .head = 0,
        .method = method
    };

    BBNode root = {
        .level = 0,
        .cost = 0,
        .benefit = 0,
        .bound = 0,
        .mask = 0
    };

    root.bound = calculateBound(
        &root, sorted, n, budget
    );

    if (!frontierPush(&frontier, root)) {
        printf("Memory allocation failed.\n");
        return;
    }

    long long bestBenefit = 0;
    int bestCost = 0;
    uint32_t bestMask = 0;

    unsigned long long explored = 0;
    unsigned long long pruned = 0;

    while (!frontierEmpty(&frontier)) {

        BBNode current = frontierPop(&frontier);
        explored++;

        if (current.bound <= (double)bestBenefit) {
            pruned++;
            continue;
        }

        if (current.level >= n)
            continue;

        int index = current.level;

        // Include the current intervention
        BBNode include = current;
        include.level = index + 1;
        include.cost += sorted[index].item.cost;
        include.benefit += sorted[index].item.benefit;
        include.mask |= (1u << index);

        if (include.cost <= budget) {

            if (include.benefit > bestBenefit) {
                bestBenefit = include.benefit;
                bestCost = include.cost;
                bestMask = include.mask;
            }

            include.bound = calculateBound(
                &include, sorted, n, budget
            );

            if (include.bound > (double)bestBenefit &&
                include.level < n) {

                if (!frontierPush(&frontier, include)) {
                    printf("Memory allocation failed.\n");
                    free(frontier.data);
                    return;
                }
            }
            else {
                pruned++;
            }
        }
        else {
            pruned++;
        }

        // Exclude the current intervention
        BBNode exclude = current;
        exclude.level = index + 1;

        exclude.bound = calculateBound(
            &exclude, sorted, n, budget
        );

        if (exclude.bound > (double)bestBenefit &&
            exclude.level < n) {

            if (!frontierPush(&frontier, exclude)) {
                printf("Memory allocation failed.\n");
                free(frontier.data);
                return;
            }
        }
        else {
            pruned++;
        }
    }

    const char *methodName =
        method == BB_FIFO ? "FIFO" :
        method == BB_LIFO ? "LIFO" :
        "BEST-BOUND";

    printf("\n===== %s BRANCH-AND-BOUND =====\n",
           methodName);

    printf("Available Budget: %d lakh\n", budget);
    printf("Maximum Benefit: %lld\n", bestBenefit);

    printf("\nSelected Interventions:\n");

    for (int i = 0; i < n; i++) {
        if (bestMask & (1u << i)) {
            printf(
                "%s | Cost: %d | Benefit: %d\n",
                sorted[i].item.name,
                sorted[i].item.cost,
                sorted[i].item.benefit
            );
        }
    }

    printf("\nTotal Budget Used: %d lakh\n",
           bestCost);

    printf("Nodes Explored: %llu\n", explored);
    printf("Branches Pruned: %llu\n", pruned);

    free(frontier.data);
}
