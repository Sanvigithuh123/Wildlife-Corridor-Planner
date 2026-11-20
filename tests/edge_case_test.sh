
#!/bin/bash
set -eu

echo "===== EDGE CASE TESTS ====="

if [ ! -x ./wildlife ]; then
    echo "ERROR: Run make first."
    exit 1
fi

passed=0
failed=0

check_case() {
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

check_case "Invalid Dijkstra source" \
    "5\n999\n3\n0\n" \
    "Invalid"

check_case "Invalid Bellman-Ford destination" \
    "11\n0\n999\n0\n" \
    "Invalid"

check_case "Negative conservation budget" \
    "6\n-10\n0\n" \
    "Invalid"

check_case "Negative Branch-and-Bound budget" \
    "12\n-10\n1\n0\n" \
    "Invalid"

# Search method 1 = Iterative Binary Search
check_case "Habitat not found" \
    "9\n999\n1\n0\n" \
    "Habitat not found."

echo ""
echo "===== TEST SUMMARY ====="
echo "Passed: $passed"
echo "Failed: $failed"

if [ "$failed" -gt 0 ]; then
    exit 1
fi

echo "All edge-case tests passed."
