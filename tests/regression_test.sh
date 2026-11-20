
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

    if output=$(printf "%b" "$input" | ./wildlife); then
        if grep -Fq "$expected" <<< "$output"; then
            echo "PASS: $name"
            passed=$((passed + 1))
            return
        fi
    fi

    echo "FAIL: $name"
    failed=$((failed + 1))
}

run_test "Kruskal MST" \
    "3\n0\n" \
    "Total Minimum Cost: 9"

run_test "Prim MST" \
    "4\n0\n" \
    "Total Minimum Cost: 9"

run_test "Dijkstra" \
    "5\n0\n3\n0\n" \
    "Minimum Route Weight: 7"

run_test "Bellman-Ford" \
    "11\n0\n3\n0\n" \
    "Minimum Route Weight: 7"

run_test "DP Knapsack" \
    "6\n10\n0\n" \
    "Maximum Benefit: 25"

run_test "FIFO Branch-and-Bound" \
    "12\n10\n1\n0\n" \
    "Maximum Benefit: 25"

run_test "LIFO Branch-and-Bound" \
    "12\n10\n2\n0\n" \
    "Maximum Benefit: 25"

run_test "Best-Bound Branch-and-Bound" \
    "12\n10\n3\n0\n" \
    "Maximum Benefit: 25"

run_test "Normal Scenario" \
    "15\n0\n" \
    "Minimum Corridor Network Cost: 9"

run_test "Corridor Shock" \
    "16\n0\n" \
    "Minimum Corridor Network Cost: 10"

run_test "Habitat Priority" \
    "21\n0\n" \
    "Highest Priority Habitat: Forest_D"

run_test "Ford-Fulkerson" \
    "13\n0\n3\n0\n" \
    "Maximum Flow: 125"

echo ""
echo "===== TEST SUMMARY ====="
echo "Passed: $passed"
echo "Failed: $failed"

if [ "$failed" -gt 0 ]; then
    exit 1
fi

echo "All regression tests passed."
