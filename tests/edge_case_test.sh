
#!/bin/bash
set -eu

echo "===== EDGE CASE TESTS ====="

passed=0
failed=0

check_case() {
    name="$1"
    input="$2"
    expected="$3"

    output=$(printf "%b" "$input" |
        ./wildlife)

    if printf "%s\n" "$output" |
       grep -Fq "$expected"; then
        echo "PASS: $name"
        passed=$((passed + 1))
    else
        echo "FAIL: $name"
        failed=$((failed + 1))
    fi
}

# Invalid habitat IDs
check_case "Invalid Dijkstra source" \
    "5\n999\n3\n0\n" \
    "Invalid"

check_case "Invalid Bellman-Ford destination" \
    "15\n0\n999\n0\n" \
    "Invalid"

# Invalid budgets
check_case "Negative conservation budget" \
    "6\n-10\n0\n" \
    "Invalid"

check_case "Negative Branch-and-Bound budget" \
    "19\n-10\n1\n0\n" \
    "Invalid"

# Invalid search
check_case "Habitat not found" \
    "9\n999\n0\n" \
    "not found"

echo ""
echo "Passed: $passed"
echo "Failed: $failed"

if [ "$failed" -gt 0 ]; then
    exit 1
fi

echo "All edge-case tests passed."
