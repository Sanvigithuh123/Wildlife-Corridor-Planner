# Wildlife Corridor and Conservation Priority Planner

## Project Overview

The Wildlife Corridor and Conservation Priority Planner is a modular C application designed to analyze habitat connectivity, identify minimum-cost wildlife corridors, optimize conservation budgets, and support conservation resource planning.

The system uses graph algorithms, greedy methods, dynamic programming, backtracking, branch-and-bound, network flow and string matching.

## Objectives

- Represent wildlife habitats and corridors using graph structures.
- Identify minimum-cost habitat connections.
- Find shortest wildlife routes.
- Optimize conservation interventions within a fixed budget.
- Analyze corridor movement capacities.
- Schedule conservation activities and assign field teams.
- Compare algorithmic strategies using performance benchmarks.

## Technologies

- Programming Language: C (C11)
- Compiler: GCC / Clang
- Development Environment: Visual Studio Code
- Build System: Make
- Data Format: CSV
- Export Formats: CSV and JSON
- Version Control: Git and GitHub

## Main Algorithms

**Graph Algorithms:** Prim, Kruskal, Dijkstra, Bellman-Ford and Floyd-Warshall.

**Dynamic Programming:** 0/1 Knapsack, LCS and bounded TSP.

**Sorting and Searching:** Merge Sort, Quick Sort, Iterative Binary Search and Recursive Binary Search.

**Greedy Algorithms:** Activity Selection, Interval Partitioning and Huffman Coding.

**Backtracking and Branch-and-Bound:** Subset Sum, FIFO, LIFO and Best-Bound Knapsack.

**Network Algorithms:** Ford-Fulkerson and Maximum Bipartite Matching.

**String Matching:** Naive, Rabin-Karp, Finite Automaton and KMP.

**Additional Analysis:** Maximum Subarray.

## Compilation

Run:

```bash
make
```

## Execution

```bash
make run
```

## Testing

```bash
make test
```

## Demonstration

```bash
bash tests/demo.sh
```

## Input Data

The application reads habitat, corridor, intervention and scheduling records from CSV files in the `data/` directory.

## Demonstration Scenarios

1. Normal habitat network operation.
2. Corridor disruption or capacity shock.
3. Large-data stress testing.

## Project Limitations

The conservation data and ecological benefit scores are illustrative. Corridor capacity is modeled using simplified network-flow assumptions. Exact TSP is restricted to small instances because of its exponential complexity.

## Future Improvements

- GIS-based habitat visualization.
- Real geographic coordinates and spatial datasets.
- Species-specific movement and habitat suitability models.
- Improved large-scale graph indexing.
- More advanced capacity-constrained corridor planning.

## Author

Individual DAA Project

## License

For academic and educational use.
