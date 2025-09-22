#!/bin/bash

# This script checks if all words in .cspell_ignored are actually used in the project's text files.
# It performs a case-insensitive search for partial word matches.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common-utils.sh"

mapfile -t SEARCH_FILES < <(find . -type f ! -path '*/.*' ! -path './.*')

check_unused_word() {
    local word="$1"
    local word_found=0
    local escaped_word
    escaped_word=$(printf '%s\n' "$word" | sed 's/[][\.^$*?+(){}|]/\\&/g')

    for file in "${SEARCH_FILES[@]}"; do
        if grep -qiE "${escaped_word}" "$file" 2>/dev/null; then
            word_found=1
            break
        fi
    done

    if [[ $word_found -eq 0 ]]; then
        return 1
    fi

    return 0
}

print_info "Checking for unused words in .cspell_ignored..."
echo ""

if [[ ! -f ".cspell_ignored" ]]; then
    print_warning ".cspell_ignored not found. Skipping check."
    exit 0
fi

mapfile -t IGNORED_WORDS <.cspell_ignored

if [[ ${#IGNORED_WORDS[@]} -eq 0 ]]; then
    print_warning "No words found in .cspell_ignored."
    exit 0
fi

any_unused_words=0
for word in "${IGNORED_WORDS[@]}"; do
    if [[ -z "$word" || "$word" =~ ^# ]]; then
        continue
    fi
    if ! check_unused_word "$word"; then
        print_error "Unused word: '$word'"
        github_warning ".cspell_ignored" "Unused word detected: '$word'"
        any_unused_words=1
    fi
done

if [[ $any_unused_words -eq 1 ]]; then
    exit_with_status 2 "Warning: Unused words detected in .cspell_ignored!" "Warning: Unused words detected in .cspell_ignored!"
else
    exit_with_status 0 "All words in .cspell_ignored are used." "All words in .cspell_ignored are used."
fi
