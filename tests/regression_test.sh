
#!/bin/bash
set -eu

echo "===== WILDLIFE PLANNER REGRESSION TESTS ====="

if [ ! -x ./wildlife ]; then
    echo "ERROR: Run make first."
    exit 1
fi

passed=0
failed=0

run_test() {
    name="$1"
    input="$2"
    expected="$3"

    output=$(printf "%b" "$input" | ./wildlife)

    if printf "%s\n" "$output" |
       grep -Fq "$expected"; then
        echo "PASS: $name"
        passed=$((passed + 1))
    else
        echo "FAIL: $name"
        failed=$((failed + 1))
    fi
}

# Minimum spanning tree
run_test "Kruskal MST" \
    "3\n0\n" \
    "Total Minimum Cost: 9"

run_test "Prim MST" \
    "4\n0\n" \
    "Total Minimum Cost: 9"

# Shortest paths
run_test "Dijkstra" \
    "5\n0\n3\n0\n" \
    "Minimum Route Weight: 7"

run_test "Bellman-Ford" \
    "15\n0\n3\n0\n" \
    "Minimum Route Weight: 7"

# Budget optimization
run_test "DP Knapsack" \
    "6\n10\n0\n" \
    "Maximum Benefit: 25"

run_test "FIFO Branch-and-Bound" \
    "19\n10\n1\n0\n" \
    "Maximum Benefit: 25"

run_test "LIFO Branch-and-Bound" \
    "19\n10\n2\n0\n" \
    "Maximum Benefit: 25"

run_test "Best-Bound Branch-and-Bound" \
    "19\n10\n3\n0\n" \
    "Maximum Benefit: 25"

# Conservation scenarios
run_test "Normal Scenario" \
    "23\n0\n" \
    "Minimum Corridor Network Cost: 9"

run_test "Corridor Shock" \
    "24\n0\n" \
    "Minimum Corridor Network Cost: 10"

# Conservation priority
run_test "Habitat Priority" \
    "29\n0\n" \
    "Highest Priority Habitat: Forest_D"

# Maximum flow
run_test "Ford-Fulkerson" \
    "20\n0\n3\n0\n" \
    "Maximum Flow: 125"

echo ""
echo "===== TEST SUMMARY ====="
echo "Passed: $passed"
echo "Failed: $failed"

if [ "$failed" -gt 0 ]; then
    exit 1
fi

echo "All regression tests passed."
