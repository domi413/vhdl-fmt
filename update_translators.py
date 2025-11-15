#!/usr/bin/env python3
import glob
import os
import re


def update_translator_file(filepath):
    """Update a translator file to use references instead of pointers"""
    with open(filepath, "r") as f:
        content = f.read()

    original_content = content

    # Update function signatures: *ctx) -> &ctx)
    content = re.sub(
        r"(\w+)\(vhdlParser::[^)]+\*ctx\)", r"\1(vhdlParser::\2&ctx)", content
    )

    # Update function calls: function(ctx) -> function(*ctx) for calls that pass pointers
    # But we need to be careful - only update calls where the argument is a pointer
    # This is tricky, so let's handle it manually for now

    if content != original_content:
        with open(filepath, "w") as f:
            f.write(content)
        print(f"Updated {filepath}")
        return True
    return False


def main():
    # Find all translator files
    translator_files = glob.glob("src/builder/translators/*.cpp", recursive=True)

    updated_count = 0
    for filepath in translator_files:
        if update_translator_file(filepath):
            updated_count += 1

    print(f"Updated {updated_count} translator files")


if __name__ == "__main__":
    main()
