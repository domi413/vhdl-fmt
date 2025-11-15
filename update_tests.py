#!/usr/bin/env python3
import re
import glob

def update_test_file(filepath):
    """Update a test file to use std::get instead of std::get_if"""
    with open(filepath, 'r') as f:
        content = f.read()

    original_content = content

    # Pattern to match: const auto *var = std::get_if<Type>(&expr);
    # Replace with: const auto& var = std::get<Type>(expr);
    pattern = r'const auto \*(\w+)\s*=\s*std::get_if<([^>]+)>\(&([^;]+)\);'
    replacement = r'const auto& \1 = std::get<\2>(\3);'

    content = re.sub(pattern, replacement, content)

    # Remove REQUIRE(var != nullptr); lines that immediately follow
    lines = content.split('\n')
    new_lines = []

    i = 0
    while i < len(lines):
        line = lines[i]

        # Check if this line is a REQUIRE with != nullptr
        require_match = re.match(r'\s*REQUIRE\(([^!]+)\s*!=\s*nullptr\);', line)
        if require_match:
            # Skip this line (remove the null check)
            i += 1
            continue

        # Replace -> with . for member access
        line = line.replace('->', '.')

        new_lines.append(line)
        i += 1

    content = '\n'.join(new_lines)

    # Remove unused <variant> include if present
    content = re.sub(r'#include\s*<variant>\s*\n', '', content)

    if content != original_content:
        with open(filepath, 'w') as f:
            f.write(content)
        print(f"Updated {filepath}")
        return True
    return False

def main():
    # Find all test files
    test_files = glob.glob('tests/**/*.cpp', recursive=True)

    updated_count = 0
    for filepath in test_files:
        if update_test_file(filepath):
            updated_count += 1

    print(f"Updated {updated_count} files")

if __name__ == '__main__':
    main()