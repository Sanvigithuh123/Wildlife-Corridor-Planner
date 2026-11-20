
# Wildlife Corridor and Conservation Priority Planner

A modular C11 conservation-planning prototype for habitat connectivity, low-cost corridor selection, shortest-route analysis, conservation budget allocation, field operations, and disruption scenarios.

## Scope and Features

- **Habitat Management:** Merge Sort, Quick Sort, iterative and recursive Binary Search, and threat/population-based conservation priority ranking.
- **Corridor Planning:** Kruskal and Prim minimum spanning trees; Dijkstra and Bellman-Ford shortest paths; Ford-Fulkerson (Edmonds-Karp) maximum directed flow.
- **Budget Optimization:** 0/1 Knapsack Dynamic Programming and FIFO, LIFO, and Best-Bound Branch-and-Bound.
- **Field Operations:** Activity Selection and Maximum Bipartite Matching for conservation team-task allocation.
- **Network Analytics:** DSU and BFS connectivity analysis and synthetic graph stress benchmarking.
- **Product Features:** Menu-driven CLI, CSV input, CSV/JSON network export, recommendation CSV files, normal and corridor-shock scenarios, reusable static library, and automated testing.

This project follows a reduced conservation-focused algorithm scope. Additional algorithms from the original DAA syllabus are excluded.

## Technologies Used

- Programming Language: C (C11)
- Compiler: GCC / Clang
- IDE: Visual Studio Code
- Build System: Make
- Version Control: Git and GitHub
- Data Formats: CSV and JSON
- Platform: macOS, with portable C source code

## Build and Run

Open the project folder in VS Code.

Compile:

```bash
make clean
make
```

Run:

```bash
./wildlife
```

The build generates:

- `wildlife` — executable application.
- `libwildlife.a` — reusable static library containing the compiled project modules.

Run commands from the project root because dataset paths are relative.

## Input Datasets

| File | Description |
|---|---|
| `data/habitats.csv` | Habitat ID, name, population and threat level |
| `data/corridors.csv` | Source, destination, cost, capacity and availability |
| `data/interventions.csv` | Intervention ID, name, cost and benefit |
| `data/activities.csv` | Conservation activity scheduling records |

The datasets are illustrative and used for academic demonstrations.

Corridor costs represent graph weights and do not necessarily correspond to physical distances.

Maximum Flow treats corridors as directed arcs, while MST and shortest-path algorithms treat them as undirected links.

## Main Menu

| Option | Function |
|---|---|
| 1 | Display Habitats |
| 2 | Display Corridors |
| 3 | Kruskal Minimum Spanning Tree |
| 4 | Prim Minimum Spanning Tree |
| 5 | Dijkstra Shortest Path |
| 6 | 0/1 Knapsack Budget Optimization |
| 7 | Merge Sort |
| 8 | Quick Sort |
| 9 | Binary Search |
| 10 | Conservation Activity Selection |
| 11 | Bellman-Ford Shortest Path |
| 12 | Branch-and-Bound Budget Optimization |
| 13 | Ford-Fulkerson Maximum Flow |
| 14 | Maximum Bipartite Matching |
| 15 | Normal Conservation Scenario |
| 16 | Corridor Shock Scenario |
| 17 | Sorting Benchmark |
| 18 | Export Network to CSV |
| 19 | Export Network to JSON |
| 20 | Large-Data Stress Benchmark |
| 21 | Conservation Priority Ranking |
| 22 | Export Conservation Recommendations |
| 0 | Exit |

## Main Algorithm Comparisons

### Prim vs Kruskal

Both algorithms identify minimum-cost habitat connections.

For the sample network:

- Kruskal MST cost: 9
- Prim MST cost: 9
- Recommended corridors: 3

### Dijkstra vs Bellman-Ford

Both algorithms find the minimum route cost between habitats.

For habitat 0 to habitat 3:

- Dijkstra route cost: 7
- Bellman-Ford route cost: 7

Different routes with equal minimum costs are valid.

### Dynamic Programming vs Branch-and-Bound

For a conservation budget of 10 lakh:

- DP Knapsack maximum benefit: 25
- FIFO Branch-and-Bound: 25
- LIFO Branch-and-Bound: 25
- Best-Bound Branch-and-Bound: 25

### Merge Sort vs Quick Sort

Both algorithms correctly sort habitats according to the selected attribute.

### DSU vs BFS

Both algorithms identify connected components. Their performance is compared using synthetic graphs of different sizes.

## Conservation Scenarios

### Scenario 1: Normal Operation

The original network contains:

- 4 habitats
- 5 corridors
- 1 connected component
- Total available edge capacity: 205
- Minimum corridor network cost: 9

### Scenario 2: Corridor Shock

Corridor 0–1 is disabled.

After the disruption:

- Available corridors decrease from 5 to 4.
- Total edge capacity decreases from 205 to 155.
- Minimum corridor cost increases from 9 to 10.
- The network remains connected.

### Scenario 3: Large-Data Stress

Synthetic networks are generated with up to:

- 5,000 habitats
- 10,000 corridors

DSU and BFS are compared for connectivity analysis.

All recorded component-count checks passed.

## Conservation Priority Ranking

Habitats are ranked using a weighted combination of threat severity and population vulnerability.

The illustrative priority formula uses:

- 70% threat severity
- 30% population vulnerability

Forest_D receives the highest priority score of 74.00 in the sample dataset.

## Exported Results

The application generates:

- `output/network.csv`
- `output/network.json`
- `output/recommended_corridors.csv`
- `output/recommended_interventions.csv`
- `output/graph_stress.csv`
- `output/algorithm_comparison.txt`

The exports contain network attributes and algorithm recommendations.

Full GIS geometry and coordinate reference systems are not currently implemented.

## Testing

Run:

```bash
make test
bash tests/regression_test.sh
bash tests/edge_case_test.sh
sh tests/recommendation_test.sh
bash tests/algorithm_comparison.sh
bash tests/demo.sh
```

Recorded verification results:

- Smoke tests: 5/5 passed
- Regression tests: 12/12 passed
- Edge-case tests: 5/5 passed
- Recommendation export test: passed
- Algorithm comparison script: completed
- Demonstration script: completed

These are primarily CLI-level integration and regression tests using sample datasets.

## Documentation

- `docs/complexity_report.md` — Complexity and performance analysis.
- `docs/design_user_guide.md` — Architecture and user instructions.
- `docs/presentation_demo.md` — Demonstration procedure.

## Limitations

- No species-specific ecological suitability model.
- No geographic coordinates or complete GIS geometry.
- Conservation priority weights are illustrative.
- Large-data benchmarking currently focuses on connectivity.
- Some algorithms have bounded input sizes.
- Comprehensive algorithm-level memory benchmarking is not yet implemented.

## Future Scope

- GIS visualization and geographic datasets.
- Species-specific habitat suitability modeling.
- Advanced conservation optimization constraints.
- Larger graph datasets and scalable graph representations.
- Improved performance benchmarking.

## Project Information

**Project:** Wildlife Corridor and Conservation Priority Planner

**Implementation:** C

**Project Type:** Individual Design and Analysis of Algorithms Project

**License:** Educational use
