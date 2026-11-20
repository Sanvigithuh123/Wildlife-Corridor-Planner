
#!/bin/bash
set -eu

echo "===== WILDLIFE CORRIDOR PLANNER DEMO ====="

make

echo ""
echo "1. Minimum-cost corridor planning"
printf "3\n4\n0\n" | ./wildlife

echo ""
echo "2. Shortest wildlife route"
printf "5\n0\n3\n0\n" | ./wildlife

echo ""
echo "3. Conservation budget optimization"
printf "6\n10\n0\n" | ./wildlife

echo ""
echo "4. Conservation activity scheduling"
printf "10\n0\n" | ./wildlife

echo ""
echo "5. Maximum corridor flow"
printf "13\n0\n3\n0\n" | ./wildlife

echo ""
echo "6. Normal and shock scenarios"
printf "15\n16\n0\n" | ./wildlife

echo ""
echo "7. Conservation priority ranking"
printf "21\n0\n" | ./wildlife

echo ""
echo "8. Export conservation recommendations"
printf "22\n10\n0\n" | ./wildlife

echo ""
echo "===== DEMO COMPLETED ====="
