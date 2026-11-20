
#!/bin/bash
set -eu

mkdir -p output

if [ ! -x ./wildlife ]; then
    echo "Executable missing. Run make first."
    exit 1
fi

REPORT="output/algorithm_comparison.txt"

echo "WILDLIFE CORRIDOR ALGORITHM COMPARISONS" > "$REPORT"
echo "======================================" >> "$REPORT"

run_case() {
    title="$1"
    input="$2"

    echo "" >> "$REPORT"
    echo "===== $title =====" >> "$REPORT"

    printf "%b" "$input" | ./wildlife >> "$REPORT"
}

# 1. Minimum Spanning Tree comparison
run_case "KRUSKAL MST" "3\n0\n"
run_case "PRIM MST" "4\n0\n"

# 2. Shortest path comparison
run_case "DIJKSTRA 0 TO 3" "5\n0\n3\n0\n"
run_case "BELLMAN-FORD 0 TO 3" "15\n0\n3\n0\n"

# 3. Budget optimization comparison
run_case "DP KNAPSACK - BUDGET 10" "6\n10\n0\n"

run_case "FIFO BRANCH AND BOUND" \
    "19\n10\n1\n0\n"

run_case "LIFO BRANCH AND BOUND" \
    "19\n10\n2\n0\n"

run_case "BEST-BOUND BRANCH AND BOUND" \
    "19\n10\n3\n0\n"

# 4. Sorting comparison
run_case "MERGE SORT BY THREAT" "7\n3\n0\n"
run_case "QUICK SORT BY THREAT" "8\n3\n0\n"

echo ""
echo "Comparison runs completed."
echo "Results saved to: $REPORT"
echo ""
echo "Review results for errors and algorithm agreement."
