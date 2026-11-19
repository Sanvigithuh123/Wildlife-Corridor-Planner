# Complexity Analysis Report

## 1. Introduction

The Wildlife Corridor and Conservation Priority Planner is a C-based conservation informatics application that uses graph algorithms, greedy methods, divide-and-conquer, dynamic programming, backtracking, branch-and-bound, network flow and string matching.

The objective of this report is to analyze the theoretical and empirical performance of the implemented algorithms and evaluate their suitability for conservation planning.

The analysis considers time complexity, auxiliary space complexity, correctness, scalability and solution quality. Algorithms solving the same optimization problem are compared using identical datasets.

The report also discusses computational complexity classes, recurrence relations and a polynomial-time reduction relevant to conservation budget optimization.


## 2. Theoretical Time and Space Complexity

Let:

- V = number of habitat vertices
- E = number of corridor edges
- n = number of input elements or interventions
- B = integer conservation budget
- m = pattern length
- T = number of teams
- A = number of tasks

| Algorithm | Time Complexity | Auxiliary Space |
|---|---|---|
| Merge Sort | O(n log n) | O(n) |
| Quick Sort | O(n²) worst case | O(n) worst-case recursion |
| Iterative Binary Search | O(log n) | O(1) |
| Recursive Binary Search | O(log n) | O(log n) |
| Maximum Subarray (Divide and Conquer) | O(n log n) | O(log n) |
| Kruskal | O(E log E + EV + V) | O(E + V) |
| Prim (Adjacency Matrix) | O(V² + EV) | O(V²) |
| Dijkstra (Adjacency Matrix) | O(V² + EV) | O(V²) |
| 0/1 Knapsack DP | O(nB) | O(nB) |
| Activity Selection | O(n²) | O(n) |
| Interval Partitioning | O(n²) | O(n) |
| Huffman Coding | O(n + k²) with fixed alphabet size k ≤ 256 | O(k²) |
| LCS | O(mn) | O(mn) |
| Bellman-Ford | O(V²E) | O(V) |
| Floyd-Warshall | O(V³ + EV) | O(V²) |
| TSP (Bitmask DP) | O(n²2ⁿ + n³ + En) | O(n2ⁿ + n²) |
| Subset Sum (Backtracking) | O(2ⁿ) | O(n) |
| Branch-and-Bound Knapsack | Exponential worst case | Depends on search frontier |
| Ford-Fulkerson (Edmonds-Karp, matrix) | O(V³E) conservative bound | O(V²) |
| Maximum Bipartite Matching (DFS) | O(T²A) | O(TA) |
| Naive String Matching | O(nm) worst case | O(1) |
| Rabin-Karp | O(n+m) expected, O(nm) worst | O(1) |
| Finite Automaton Matching | O(256m+n) | O(256m) |
| KMP | O(n+m) | O(m) |

These bounds describe the implemented approaches, including preprocessing overhead where relevant.

The Kruskal, Prim, Dijkstra, Bellman-Ford, Floyd-Warshall and TSP implementations currently perform linear habitat-ID lookups. These introduce additional overhead compared with optimized textbook implementations.

For Huffman Coding, k denotes the number of distinct symbols. The fixed 256-byte alphabet makes its alphabet-dependent costs bounded.

The Knapsack DP implementation has pseudo-polynomial time complexity because its runtime depends on the numeric value of B.


## 3. Recurrence Relations

### 3.1 Merge Sort

Merge Sort divides an array into two halves and merges the sorted halves in linear time.

Recurrence:

\[
T(n)=2T(n/2)+\Theta(n)
\]

Using the Master Theorem:

\[
a=2,\quad b=2,\quad f(n)=\Theta(n)
\]

Since:

\[
n^{\log_b a}=n
\]

the recurrence belongs to Master Theorem Case 2.

Therefore:

\[
T(n)=\Theta(n\log n)
\]

### 3.2 Binary Search

Binary Search reduces the search space by half during each step.

\[
T(n)=T(n/2)+\Theta(1)
\]

Therefore:

\[
T(n)=\Theta(\log n)
\]

### 3.3 Maximum Subarray

The divide-and-conquer Maximum Subarray algorithm recursively solves two halves and computes the maximum crossing sum in linear time.

\[
T(n)=2T(n/2)+\Theta(n)
\]

Therefore:

\[
T(n)=\Theta(n\log n)
\]

### 3.4 Quick Sort

Quick Sort partitions the array around a pivot.

For balanced partitions:

\[
T(n)=2T(n/2)+\Theta(n)
\]

Therefore:

\[
T(n)=\Theta(n\log n)
\]

For highly unbalanced partitions:

\[
T(n)=T(n-1)+\Theta(n)
\]

Expanding the recurrence:

\[
T(n)=n+(n-1)+\cdots+1
\]

Therefore:

\[
T(n)=\Theta(n^2)
\]

### 3.5 Subset Sum Backtracking

Each intervention creates two possible decisions: include or exclude.

In the worst case:

\[
T(n)=2T(n-1)+\Theta(1)
\]

Therefore:

\[
T(n)=O(2^n)
\]

Pruning can reduce the number of explored states in practice, but it does not eliminate the exponential worst case.


## 5. Correctness Evidence

Correctness was evaluated by comparing algorithm outputs with independently determined expected results for small instances.

| Algorithm | Test Case | Expected Result |
|---|---|---|
| Kruskal | Four habitats, five corridors | MST cost = 9 |
| Prim | Same habitat graph | MST cost = 9 |
| Dijkstra | Forest A to Forest D | Shortest route cost = 7 |
| Bellman-Ford | Forest A to Forest D | Shortest route cost = 7 |
| 0/1 Knapsack | Budget = 10 lakh | Maximum benefit = 25 |
| TSP | Four-habitat inspection tour | Minimum tour cost = 14 |
| Subset Sum | Target budget = 10 lakh | Feasible subset exists |
| Ford-Fulkerson | Source A, sink D | Maximum flow = 125 |
| String Matching | Text ABCDABCDABC, pattern ABC | Matches at 0, 4 and 8 |

Additional correctness checks should include disconnected graphs, invalid habitat IDs, blocked corridors, empty inputs, duplicate records and infeasible budgets.

For algorithm comparisons, agreement in output is useful evidence, but independent small-instance oracles are also required to verify optimality.


## 7. Recommended Production Strategy

For a practical wildlife conservation planning system, algorithm selection should depend on graph density, input size, constraints and the required output.

**Minimum-Cost Corridor Planning:** Kruskal is a suitable default for sparse corridor graphs because it works naturally with edge lists. Prim is an appropriate alternative, especially with an efficient priority queue.

**Shortest-Path Routing:** Dijkstra is recommended for nonnegative route weights. Bellman-Ford is useful when signed weights are meaningful and negative-cycle detection is required. Floyd-Warshall is suitable for smaller networks requiring all-pairs distances.

**Conservation Budget Optimization:** Dynamic-programming 0/1 Knapsack is suitable for moderate integer budgets. Branch-and-Bound provides an exact alternative when useful upper bounds reduce the search space.

**Habitat Data Management:** Merge Sort provides stable and predictable sorting performance. Binary Search supports efficient lookup after a sorted habitat index has been constructed.

**Corridor Capacity Analysis:** Edmonds-Karp provides a straightforward maximum-flow implementation. Larger networks may benefit from more advanced maximum-flow algorithms.

**Conservation Scheduling:** Activity Selection and Interval Partitioning provide efficient greedy approaches when activities have fixed time intervals and interchangeable teams.

**Inspection Tour Planning:** Exact bitmask TSP is suitable only for small habitat sets. Larger instances should use approximation or heuristic methods, with solution quality reported separately.

The recommended production architecture separates data validation, graph construction, optimization algorithms, scenario analysis and result exports into reusable modules.

All recommendations must respect the ecological and operational constraints represented in the data. Minimum-cost connectivity alone does not guarantee ecological suitability or capacity feasibility.
