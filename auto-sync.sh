
#!/bin/bash

cd "$(dirname "$0")" || exit 1

while true; do
    git add -A

    if ! git diff --cached --quiet; then
        git commit -m "Auto-save: $(date '+%Y-%m-%d %H:%M:%S')" &&
        git push origin main
    fi

    sleep 60
done
