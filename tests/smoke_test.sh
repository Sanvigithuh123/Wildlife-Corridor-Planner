
#!/bin/bash
set -eu

echo "===== WILDLIFE PLANNER SMOKE TESTS ====="

if [ ! -x ./wildlife ]; then
    echo "Executable not found."
    exit 1
fi

run_test() {
    name="$1"
    input="$2"
    expected="$3"

    output=$(printf "%b" "$input" | ./wildlife)

    if printf "%s" "$output" | grep -Fq "$expected"; then
        echo "PASS: $name"
    else
        echo "FAIL: $name"
        exit 1
    fi
}

run_test "Kruskal MST" \
    "3\n0\n" \
    "Total Minimum Cost: 9"

run_test "Prim MST" \
    "4\n0\n" \
    "Total Minimum Cost: 9"

run_test "Dijkstra Shortest Path" \
    "5\n0\n3\n0\n" \
    "Minimum Route Weight: 7"

run_test "Knapsack Budget" \
    "6\n10\n0\n" \
    "Maximum Benefit: 25"



run_test "Maximum Flow" \
    "13\n0\n3\n0\n" \
    "Maximum Flow: 125"

echo "All smoke tests passed."
