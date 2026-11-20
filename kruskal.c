
#include <stdio.h>
#include <stdlib.h>
#include "kruskal.h"

typedef struct {
    Corridor edge;
    int u, v;
} Edge;

static int indexOf(const Habitat h[], int n, int id) {
    for (int i = 0; i < n; i++)
        if (h[i].id == id)
            return i;
    return -1;
}

static int root(int p[], int x) {
    return p[x] == x ? x : (p[x] = root(p, p[x]));
}

static int cmp(const void *a, const void *b) {
    const Edge *x = a, *y = b;
    return (x->edge.cost > y->edge.cost) -
           (x->edge.cost < y->edge.cost);
}

int calculateKruskalMST(
    const Habitat h[], int n,
    const Corridor c[], int m,
    MSTResult *out
) {
    if (!h || !c || !out || n < 1 ||
        n > MAX_MST_EDGES || m < 0)
        return -1;

    *out = (MSTResult){0};

    Edge *edges = malloc(
        (size_t)(m ? m : 1) * sizeof(*edges)
    );
    if (!edges) return -1;

    int k = 0;
    for (int i = 0; i < m; i++) {
        if (c[i].available) {
            int u = indexOf(h, n, c[i].source);
            int v = indexOf(h, n, c[i].destination);

            if (u < 0 || v < 0 || c[i].cost < 0) {
                free(edges);
                return -1;
            }
            edges[k++] = (Edge){c[i], u, v};
        }
    }

    qsort(edges, (size_t)k, sizeof(*edges), cmp);

    int p[MAX_MST_EDGES];
    int rank[MAX_MST_EDGES] = {0};

    for (int i = 0; i < n; i++)
        p[i] = i;

    for (int i = 0; i < k &&
         out->selectedCount < n - 1; i++) {

        int a = root(p, edges[i].u);
        int b = root(p, edges[i].v);

        if (a == b) continue;

        if (rank[a] < rank[b])
            p[a] = b;
        else if (rank[a] > rank[b])
            p[b] = a;
        else {
            p[b] = a;
            rank[a]++;
        }

        out->selected[out->selectedCount++] =
            edges[i].edge;
        out->totalCost += edges[i].edge.cost;
    }

    out->connected = (out->selectedCount == n - 1);
    free(edges);
    return 0;
}

void kruskalMST(
    Habitat h[], int n,
    Corridor c[], int m
) {
    MSTResult r;

    if (calculateKruskalMST(h, n, c, m, &r)) {
        puts("Kruskal calculation failed.");
        return;
    }

    puts("\n===== KRUSKAL MST =====");
    puts("Selected Wildlife Corridors:");

    for (int i = 0; i < r.selectedCount; i++) {
        Corridor e = r.selected[i];

        printf("%s -> %s | Cost: %d\n",
            h[indexOf(h, n, e.source)].name,
            h[indexOf(h, n, e.destination)].name,
            e.cost);
    }

    if (r.connected) {
        printf("\nTotal Minimum Cost: %lld\n",
               r.totalCost);
        puts("All habitats connected successfully.");
    } else {
        puts("All habitats cannot be connected.");
        printf("Selected edges: %d\nForest cost: %lld\n",
               r.selectedCount, r.totalCost);
    }
}
