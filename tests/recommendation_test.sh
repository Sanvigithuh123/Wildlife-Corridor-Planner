
#!/bin/sh
set -eu

echo "===== RECOMMENDATION EXPORT TEST ====="

if [ ! -x ./wildlife ]; then
    echo "ERROR: Run make first."
    exit 1
fi

mkdir -p output

printf '22\n10\n0\n' |
    ./wildlife > output/recommendation_test_log.txt

grep -q 'MST cost: 9' \
    output/recommendation_test_log.txt

grep -q 'Maximum benefit: 25' \
    output/recommendation_test_log.txt

test -f output/recommended_corridors.csv
test -f output/recommended_interventions.csv

# Header + 3 selected corridors
test "$(wc -l < output/recommended_corridors.csv)" -eq 4

test -s output/recommended_interventions.csv

echo "PASS: Recommendation export"
echo "MST cost: 9"
echo "Maximum benefit: 25"
echo "Both CSV files generated successfully."
