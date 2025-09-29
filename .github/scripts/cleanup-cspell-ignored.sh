#!/bin/bash

# This script removes unused words from the .cspell_ignored file by
# checking for case-insensitive partial matches in the project's files.

mapfile -t SEARCH_FILES < <(find . -type f ! -path '*/.*' ! -name '.cspell_ignored')

check_unused_word() {
    local word="$1"
    local escaped_word
    escaped_word=$(printf '%s\n' "$word" | sed 's/[][\.^$*?+(){}|]/\\&/g')

    # Grep for the word in all files, ignoring case.
    grep -qiE "${escaped_word}" "${SEARCH_FILES[@]}" 2>/dev/null
}

if [ ! -f .cspell_ignored ]; then
    echo "Error: .cspell_ignored not found!"
    exit 1
fi

temp_file=$(mktemp)
unused_count=0

while IFS= read -r line; do
    if echo "$line" | grep -q '^#\|^$'; then
        echo "$line" >>"$temp_file"
    elif [ -n "$line" ]; then
        if check_unused_word "$line"; then
            echo "$line" >>"$temp_file"
        else
            echo "Removing unused word: '$line'"
            unused_count=$((unused_count + 1))
        fi
    fi
done <.cspell_ignored

if [ "$unused_count" -gt 0 ]; then
    mv "$temp_file" .cspell_ignored
    echo "✓ Cleaned up .cspell_ignored - removed $unused_count unused words."
else
    rm -f "$temp_file"
    echo "✓ No unused words found in .cspell_ignored"
fi

exit 0
