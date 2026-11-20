#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "benchmark.h"
#include "sorting.h"

#define RUNS 20

typedef struct { int u, v; } Edge;

static double ms(clock_t a, clock_t b) {
    return 1000.0 * (double)(b - a) / CLOCKS_PER_SEC;
}
static unsigned int rng_state = 123456789u;
static unsigned int next_random(void) {
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state;
}
static int sorted_correct(const Habitat *a, int n) {
    for (int i = 1; i < n; ++i)
        if (a[i-1].threatLevel > a[i].threatLevel) return 0;
    return 1;
}
void benchmarkSorting(const Habitat habitats[], int habitatCount) {
    if (!habitats || habitatCount <= 0) { puts("No habitats available."); return; }
    Habitat *copy = malloc((size_t)habitatCount * sizeof(*copy));
    if (!copy) { puts("Allocation failed."); return; }
    double times[2] = {0,0};
    int correct[2] = {1,1};
    for (int method = 0; method < 2; ++method) {
        for (int run = 0; run < RUNS; ++run) {
            memcpy(copy, habitats, (size_t)habitatCount * sizeof(*copy));
            clock_t start = clock();
            if (method == 0) mergeSort(copy, habitatCount, SORT_BY_THREAT);
            else quickSort(copy, habitatCount, SORT_BY_THREAT);
            times[method] += ms(start, clock());
            if (!sorted_correct(copy, habitatCount)) correct[method] = 0;
        }
    }
    printf("\n===== SORTING BENCHMARK =====\nHabitats: %d, runs: %d\n", habitatCount, RUNS);
    printf("Merge Sort: %.6f ms/run, correct: %s\n", times[0]/RUNS, correct[0]?"YES":"NO");
    printf("Quick Sort: %.6f ms/run, correct: %s\n", times[1]/RUNS, correct[1]?"YES":"NO");
    free(copy);
}
static int root(int *parent, int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}
static int count_dsu(int n, const Edge *edges, int m) {
    int *parent = malloc((size_t)n * sizeof(*parent));
    unsigned char *rank = calloc((size_t)n, sizeof(*rank));
    if (!parent || !rank) { free(parent); free(rank); return -1; }
    for (int i = 0; i < n; ++i) parent[i] = i;
    int components = n;
    for (int i = 0; i < m; ++i) {
        int a = root(parent, edges[i].u), b = root(parent, edges[i].v);
        if (a == b) continue;
        if (rank[a] < rank[b]) { int t = a; a = b; b = t; }
        parent[b] = a;
        if (rank[a] == rank[b]) ++rank[a];
        --components;
    }
    free(parent); free(rank);
    return components;
}
static int count_bfs(int n, const Edge *edges, int m) {
    int *head = malloc((size_t)n * sizeof(*head));
    int *to = malloc((size_t)(2*m) * sizeof(*to));
    int *next = malloc((size_t)(2*m) * sizeof(*next));
    int *queue = malloc((size_t)n * sizeof(*queue));
    unsigned char *seen = calloc((size_t)n, 1);
    if (!head || !to || !next || !queue || !seen) {
        free(head); free(to); free(next); free(queue); free(seen); return -1;
    }
    for (int i = 0; i < n; ++i) head[i] = -1;
    int k = 0;
    for (int i = 0; i < m; ++i) {
        int u = edges[i].u, v = edges[i].v;
        to[k] = v; next[k] = head[u]; head[u] = k++;
        to[k] = u; next[k] = head[v]; head[v] = k++;
    }
    int components = 0;
    for (int s = 0; s < n; ++s) {
        if (seen[s]) continue;
        ++components;
        int front = 0, back = 0;
        seen[s] = 1; queue[back++] = s;
        while (front < back) {
            int u = queue[front++];
            for (int e = head[u]; e != -1; e = next[e]) {
                int v = to[e];
                if (!seen[v]) { seen[v] = 1; queue[back++] = v; }
            }
        }
    }
    free(head); free(to); free(next); free(queue); free(seen);
    return components;
}
static int run_case(FILE *csv, int n, int m, const char *scenario) {
    Edge *edges = malloc((size_t)m * sizeof(*edges));
    if (!edges) return 0;
    /* Deterministic connected backbone, or two disconnected halves. */
    int split = strcmp(scenario, "disconnected") == 0;
    int k = 0;
    for (int i = 1; i < n && k < m; ++i) {
        if (split && i == n/2) continue;
        edges[k++] = (Edge){i-1,i};
    }
    while (k < m) {
        int u = (int)(next_random() % (unsigned)n);
        int v = (int)(next_random() % (unsigned)n);
        if (split && (u < n/2) != (v < n/2)) continue;
        if (u == v) continue;
        edges[k++] = (Edge){u,v};
    }
    double dsu_ms = 0, bfs_ms = 0;
    int a = -1, b = -1;
    for (int r = 0; r < RUNS; ++r) {
        clock_t start = clock();
        a = count_dsu(n, edges, m);
        dsu_ms += ms(start, clock());
        start = clock();
        b = count_bfs(n, edges, m);
        bfs_ms += ms(start, clock());
    }
    int ok = a >= 0 && a == b && a == (split ? 2 : 1);
    printf("%-13s V=%-5d E=%-6d DSU=%-8.4fms BFS=%-8.4fms components=%d %s\n",
           scenario, n, m, dsu_ms/RUNS, bfs_ms/RUNS, a, ok?"PASS":"FAIL");
    if (csv) fprintf(csv, "%s,%d,%d,%.6f,%.6f,%d,%d,%s\n",
                     scenario,n,m,dsu_ms/RUNS,bfs_ms/RUNS,a,b,ok?"PASS":"FAIL");
    free(edges);
    return ok;
}
void benchmarkGraphSizes(void) {
    FILE *csv = fopen("output/graph_stress.csv", "w");
    if (!csv) puts("Warning: create output/ to save graph_stress.csv");
    if (csv) fputs("scenario,vertices,edges,dsu_ms,bfs_ms,dsu_components,bfs_components,status\n", csv);
    puts("\n===== GRAPH STRESS: DSU vs BFS CONNECTIVITY =====");
    puts("Synthetic undirected graphs; deterministic generator; mean of 20 CPU-time runs.");
    int passed = 1;
    passed &= run_case(csv, 20, 40, "normal");
    passed &= run_case(csv, 100, 200, "normal");
    passed &= run_case(csv, 1000, 2000, "normal");
    passed &= run_case(csv, 5000, 10000, "normal");
    passed &= run_case(csv, 1000, 2000, "disconnected");
    if (csv) { fclose(csv); puts("Saved: output/graph_stress.csv"); }
    puts(passed ? "All graph stress cases PASSED." : "Graph stress FAILED; inspect results.");
}
