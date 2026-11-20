
#!/bin/bash
set -eu

if [ ! -x ./wildlife ]; then
    echo "ERROR: Run make first."
    exit 1
fi

mkdir -p output

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

# Minimum Spanning Tree
run_case "KRUSKAL MST" "3\n0\n"
run_case "PRIM MST" "4\n0\n"

# Shortest Path
run_case "DIJKSTRA 0 TO 3" "5\n0\n3\n0\n"
run_case "BELLMAN-FORD 0 TO 3" "11\n0\n3\n0\n"

# Conservation Budget
run_case "DP KNAPSACK - BUDGET 10" "6\n10\n0\n"

run_case "FIFO BRANCH AND BOUND" \
    "12\n10\n1\n0\n"

run_case "LIFO BRANCH AND BOUND" \
    "12\n10\n2\n0\n"

run_case "BEST-BOUND BRANCH AND BOUND" \
    "12\n10\n3\n0\n"

# Habitat Sorting
run_case "MERGE SORT BY THREAT" "7\n3\n0\n"
run_case "QUICK SORT BY THREAT" "8\n3\n0\n"

echo ""
echo "Comparison runs completed."
echo "Saved: $REPORT"
echo ""
echo "Review the results for algorithm agreement."
