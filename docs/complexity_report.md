
# Complexity and Evaluation Report

## 1. Introduction

The Wildlife Corridor and Conservation Priority Planner is a C-based application designed to analyze habitat connectivity, identify minimum-cost wildlife corridors, optimize conservation budgets, calculate movement capacity, and support conservation resource planning.

This report evaluates the theoretical and empirical performance of the retained algorithms.

The analysis includes:

- Time complexity
- Auxiliary space complexity
- Recurrence relations
- Algorithm comparisons
- Empirical benchmarking
- Correctness evidence
- Computational complexity classes
- Polynomial-time reduction
- Recommended production strategies

The report covers the final reduced conservation-focused implementation.

## 2. Complexity Notation

Let:

- V = Number of habitats or graph vertices
- E = Number of corridors or graph edges
- n = Number of elements or interventions
- B = Integer conservation budget
- T = Number of conservation teams
- A = Number of activities or tasks

Auxiliary space excludes input storage unless stated otherwise.

## 3. Theoretical Complexity Analysis

| Algorithm | Time Complexity | Auxiliary Space |
|---|---|---|
| Merge Sort | O(n log n) | O(n) |
| Quick Sort | O(n log n) average, O(n²) worst | O(log n) average, O(n) worst |
| Iterative Binary Search | O(log n) | O(1) |
| Recursive Binary Search | O(log n) | O(log n) |
| Kruskal | O(E log E + EV), conservative with linear ID lookup | O(E + V) |
| Prim (Adjacency Matrix) | O(V² + EV), conservative | O(V²) |
| Dijkstra (Adjacency Matrix) | O(V² + EV), conservative | O(V²) |
| Bellman-Ford | O(VE + EV), conservative | O(V) |
| 0/1 Knapsack DP | O(nB) | O(nB) |
| Branch-and-Bound Knapsack | O(n2^n) conservative worst case | Potentially exponential |
| Activity Selection | O(n log n) with efficient sorting; O(n²) with insertion sort | O(n) |
| Ford-Fulkerson (BFS Residual Matrix) | O(V³E), conservative | O(V²) |
| Maximum Bipartite Matching (DFS Matrix) | O(T²A) | O(TA) |
| DSU Connectivity | O((V+E) alpha(V)) | O(V) |
| BFS Connectivity | O(V+E) | O(V+E) |
| Priority Scoring with Merge Sort | O(n log n) | O(n) |

These bounds include conservative preprocessing overhead where appropriate.

## 4. Recurrence Relations

### 4.1 Merge Sort

Merge Sort divides the input into two halves and merges them.

T(n) = 2T(n/2) + Theta(n)

Using the Master Theorem:

T(n) = Theta(n log n)

### 4.2 Recursive Binary Search

Binary Search reduces the search space by half.

T(n) = T(n/2) + Theta(1)

Therefore:

T(n) = Theta(log n)

### 4.3 Quick Sort

For balanced partitions:

T(n) = 2T(n/2) + Theta(n)

Therefore:

T(n) = Theta(n log n)

For highly unbalanced partitions:

T(n) = T(n-1) + Theta(n)

Therefore:

T(n) = Theta(n²)

### 4.4 Branch-and-Bound

Branch-and-Bound explores include/exclude decisions.

Without effective pruning, the search tree may contain Theta(2^n) nodes.

Bounding reduces the number of explored states in favorable instances but does not eliminate the exponential worst case.

## 5. Algorithm Correctness and Comparison

### 5.1 Prim vs Kruskal

| Algorithm | MST Cost |
|---|---|
| Kruskal | 9 |
| Prim | 9 |

Both algorithms returned the same minimum spanning-tree cost.

### 5.2 Dijkstra vs Bellman-Ford

| Algorithm | Shortest Route Cost |
|---|---|
| Dijkstra | 7 |
| Bellman-Ford | 7 |

Both algorithms returned the same shortest-path cost from habitat 0 to habitat 3.

### 5.3 DP Knapsack vs Branch-and-Bound

| Algorithm | Maximum Benefit |
|---|---|
| Dynamic Programming | 25 |
| FIFO Branch-and-Bound | 25 |
| LIFO Branch-and-Bound | 25 |
| Best-Bound Branch-and-Bound | 25 |

All methods returned the same optimal benefit for the sample budget of 10.

### 5.4 Branch-and-Bound Search Efficiency

| Method | Nodes Explored | Branches Pruned |
|---|---:|---:|
| FIFO | 15 | 12 |
| LIFO | 24 | 24 |
| Best-Bound | 10 | 9 |

Best-Bound explored the fewest nodes for this sample instance.

This does not imply that Best-Bound always performs best.

### 5.5 Merge Sort vs Quick Sort

Both algorithms produced the same ascending threat-level order:

Forest_C → Forest_A → Forest_B → Forest_D

Threat levels:

2 → 3 → 5 → 8

## 6. Conservation Scenario Analysis

### 6.1 Normal Operation

The original habitat network contains:

- 4 habitats
- 5 available corridors
- 1 connected component
- Total edge capacity: 205
- Minimum spanning-tree cost: 9

### 6.2 Corridor Constraint Shock

Corridor 0–1 was disabled.

| Measurement | Before Shock | After Shock |
|---|---:|---:|
| Available Corridors | 5 | 4 |
| Connected Components | 1 | 1 |
| Total Edge Capacity | 205 | 155 |
| Minimum Network Cost | 9 | 10 |
| Selected MST Corridors | 3 | 3 |

The network remains connected after the disruption, but its minimum connection cost increases.

Total edge capacity is the sum of available corridor capacities, not the maximum network flow.

### 6.3 Maximum Flow

Ford-Fulkerson calculated a maximum directed flow of 125 units from habitat 0 to habitat 3 on the normal dataset.

The current scenario module does not automatically recalculate maximum flow after the shock.

## 7. Empirical Large-Data Benchmark

The benchmark compares DSU and BFS connected-component analysis on synthetic undirected graphs.

Measurements were recorded over 20 CPU-time runs per generated case.

| Scenario | Habitats | Corridors | DSU (ms) | BFS (ms) | Components |
|---|---:|---:|---:|---:|---:|
| Normal | 20 | 40 | 0.002650 | 0.003600 | 1 |
| Normal | 100 | 200 | 0.004800 | 0.006200 | 1 |
| Normal | 1,000 | 2,000 | 0.017600 | 0.019450 | 1 |
| Normal | 5,000 | 10,000 | 0.061150 | 0.139850 | 1 |
| Disconnected | 1,000 | 2,000 | 0.010750 | 0.012800 | 2 |

All recorded component-count checks passed.

DSU was faster than BFS on these generated datasets.

These measurements evaluate connectivity scalability, not the large-scale performance of every conservation optimization algorithm.

## 8. Memory Complexity

Theoretical auxiliary memory requirements are listed in Section 3.

Important observations:

- Merge Sort requires additional array storage.
- Quick Sort uses recursion stack space.
- Matrix-based graph algorithms require O(V²) memory.
- Knapsack DP uses O(nB) memory.
- Branch-and-Bound memory depends on the number of active search nodes.
- DSU requires O(V) auxiliary memory.
- BFS with adjacency lists requires O(V+E) storage.

Algorithm-specific peak memory was not empirically measured for every comparison.

Whole-process memory usage is not equivalent to an algorithm's auxiliary-space complexity.

## 9. Correctness and Testing Evidence

The application uses sample-based automated tests.

| Test Category | Recorded Result |
|---|---|
| Smoke Tests | 5/5 PASS |
| Regression Tests | 12/12 PASS |
| Edge-Case Tests | 5/5 PASS |
| Recommendation Export | PASS |
| Algorithm Comparison Script | Completed |
| Demonstration Script | Completed |

Regression tests verify that previously working functionality remains correct after modifications.

Edge-case tests include invalid habitat IDs and negative conservation budgets.

These tests provide evidence for the tested inputs, but do not establish correctness for every possible graph or dataset.

## 10. Complexity Classes

### Class P

P contains decision problems solvable in polynomial time.

Examples related to the project include:

- Graph connectivity
- Minimum spanning-tree decision
- Shortest-path decision under appropriate assumptions

### Class NP

NP contains decision problems for which proposed yes-solutions can be verified in polynomial time.

### NP-Hard

An NP-hard problem is at least as hard as every problem in NP under polynomial-time reductions.

0/1 Knapsack optimization is NP-hard.

### NP-Complete

A problem is NP-complete if it belongs to NP and is NP-hard.

Examples include:

- Subset Sum decision
- 0/1 Knapsack decision
- Travelling Salesman decision

These examples are used for theoretical analysis; they are not all implemented in the final application.

## 11. Polynomial-Time Reduction

### Subset Sum to 0/1 Knapsack Decision

Consider a Subset Sum instance with positive integers:

a1, a2, ..., an

and target B.

Construct a 0/1 Knapsack decision instance where:

- Weight of item i = ai
- Value of item i = ai
- Knapsack capacity = B
- Required total value = B

A feasible Knapsack solution must satisfy:

Total weight <= B

and:

Total value >= B

Since weights and values are identical, both conditions hold exactly when:

Total selected sum = B

Therefore:

Subset Sum <=p 0/1 Knapsack Decision

This is a polynomial-time reduction.

Subset Sum is included here only as a theoretical reduction source, not as an implemented final application module.

## 12. Recommended Production Strategy

### Minimum-Cost Corridor Planning

Kruskal is suitable for sparse corridor networks represented as edge lists.

Prim provides an alternative MST strategy and correctness cross-check.

### Wildlife Routing

Dijkstra is recommended when corridor costs are nonnegative.

Bellman-Ford can be used for comparison and for signed-weight problems where appropriate.

### Conservation Budget Optimization

Dynamic Programming is suitable when integer budgets are moderate.

Best-Bound Branch-and-Bound is an alternative exact method when effective bounds reduce the search space.

### Corridor Capacity Analysis

Ford-Fulkerson with BFS-based augmenting paths is suitable for small capacity networks.

### Habitat Connectivity

DSU is suitable for repeated connectivity checks.

BFS is useful for graph traversal and independent validation.

### Conservation Scheduling

Activity Selection helps choose compatible non-overlapping activities.

Maximum Bipartite Matching assigns teams to compatible tasks.

## 13. Limitations

- The original assignment lists additional algorithms excluded from the reduced project scope.
- The dataset is illustrative and does not model detailed species-specific habitat suitability.
- Geographic coordinates and a coordinate reference system are not implemented.
- CSV network exports contain attributes rather than full GIS geometry.
- Large-data benchmarking focuses on DSU and BFS connectivity.
- Complete algorithm-level runtime and memory benchmarking remains future work.
- Additional adversarial datasets and direct function-level tests would strengthen correctness evidence.

## 14. Conclusion

The Wildlife Corridor and Conservation Priority Planner demonstrates the application of graph algorithms, greedy methods, dynamic programming, branch-and-bound, sorting, searching and network flow to conservation-planning problems.

The implemented algorithms support minimum-cost habitat connectivity, wildlife routing, conservation budget allocation, capacity analysis, field operations and disruption scenarios.

Sample comparisons show agreement between alternative algorithms solving the same problems.

The large-data benchmark demonstrates connectivity analysis on synthetic graphs containing up to 5,000 habitats and 10,000 corridors.

The project provides a modular academic prototype that can be extended with geographic data, species-specific ecological models and larger-scale optimization techniques.
