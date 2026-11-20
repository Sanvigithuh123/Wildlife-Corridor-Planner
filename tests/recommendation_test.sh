
#!/bin/sh
set -eu

mkdir -p output

printf '30\n10\n0\n' | ./wildlife > /tmp/wildlife_recommendation_test.txt

grep -q 'MST cost: 9' \
    /tmp/wildlife_recommendation_test.txt

grep -q 'Maximum benefit: 25' \
    /tmp/wildlife_recommendation_test.txt

[ "$(wc -l < output/recommended_corridors.csv)" -eq 4 ]

[ -s output/recommended_interventions.csv ]

echo 'PASS: recommendation export (MST cost 9, benefit 25)'
