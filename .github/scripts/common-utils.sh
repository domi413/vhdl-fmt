#!/bin/bash

# Common utilities for checking scripts

# Color definitions
export CYAN='\033[1;36m'
export GREEN='\033[1;92m'
export RED='\033[1;91m'
export YELLOW='\033[1;93m'
export NC='\033[0m'

# Configurable directory paths
export PREFIX_DIR="${PREFIX_DIR:-.}"

# Common functions
print_info() {
    if [ $# -eq 1 ]; then
        echo -e "${CYAN}$1${NC}"
    else
        printf "${CYAN}$1${NC}" "${@:2}"
    fi
}

print_success() {
    if [ $# -eq 1 ]; then
        echo -e "${GREEN}$1${NC}"
    else
        printf "${GREEN}$1${NC}" "${@:2}"
    fi
}

print_error() {
    if [ $# -eq 1 ]; then
        echo -e "${RED}$1${NC}"
    else
        printf "${RED}$1${NC}" "${@:2}"
    fi
}

print_warning() {
    if [ $# -eq 1 ]; then
        echo -e "${YELLOW}$1${NC}"
    else
        printf "${YELLOW}$1${NC}" "${@:2}"
    fi
}

# Function to report GitHub Actions compatible error
github_error() {
    local file="$1"
    local line="$2"
    local message="$3"

    if [[ -n "$line" ]]; then
        echo "::error file=$file,line=$line::$message"
    else
        echo "::error file=$file::$message"
    fi
}

# Function to report GitHub Actions compatible warning
github_warning() {
    local file="$1"
    local message="$2"

    echo "::warning file=$file::$message"
}

# Function to handle script exit with proper messaging
exit_with_status() {
    local exit_code="$1"
    local success_message="$2"
    local failure_message="$3"

    if [[ $exit_code -eq 0 ]]; then
        print_success "$success_message"
    elif [[ $exit_code -eq 2 ]]; then
        # We incorrectly pass not 0 in case of a warning to differ between
        # success and warning. Therefore we force exit 0
        print_warning "$failure_message"
        exit 0
    else
        print_error "$failure_message"
    fi

    exit "$exit_code"
}
