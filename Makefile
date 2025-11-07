.PHONY: all run clean conan test test-rerun test-verbose lint check-format format sort-dictionary cleanup-dictionary check-cspell-ignored docker-dev-build docker-dev docker-make docker-test docker-publish-ci

# -----------------------------
# Build Configuration
# -----------------------------
# Default preset, override with `make BUILD_TYPE=Release`
BUILD_TYPE ?= Debug
CMAKE_PRESET := conan-$(shell echo $(BUILD_TYPE) | tr A-Z a-z)

TARGET := build/$(BUILD_TYPE)/bin/vhdl_formatter
CONAN_STAMP := build/.conan.$(BUILD_TYPE).stamp
BUILD_STAMP := build/.build.$(BUILD_TYPE).stamp

SOURCES := $(shell find src tests -name '*.cpp' -o -name '*.hpp')
SOURCES_CMAKE := $(shell find src tests . -name 'CMakeLists.txt')

# -----------------------------
# Build Targets
# -----------------------------
all: $(BUILD_STAMP)

$(BUILD_STAMP): $(SOURCES) $(SOURCES_CMAKE) $(CONAN_STAMP)
	@echo "Building project ($(BUILD_TYPE))..."
	@cmake --preset $(CMAKE_PRESET)
	@cmake --build --preset $(CMAKE_PRESET)
	@touch $@
	@echo "Build complete."

CONAN_CMD := conan
$(CONAN_STAMP): conanfile.txt
	$(call check_tool,$(CONAN_CMD))
	@echo "Running Conan ($(BUILD_TYPE))..."
	@$(CONAN_CMD) install . \
		--profile:host=clang.profile \
		--profile:build=clang.profile \
		--build=missing \
		-s build_type=$(BUILD_TYPE)
	@touch $@

conan: $(CONAN_STAMP)

run: $(BUILD_STAMP)
	@./$(TARGET) ./tests/data/vhdl/simple.vhdl

test: $(BUILD_STAMP)
	@ctest --preset $(CMAKE_PRESET) --output-on-failure

test-rerun-failed: $(BUILD_STAMP)
	@ctest --preset $(CMAKE_PRESET) --rerun-failed --output-on-failure

test-verbose: $(BUILD_STAMP)
	@ctest --preset $(CMAKE_PRESET) --verbose

clean:
	@rm -rf build CMakeFiles CMakeCache.txt CMakeUserPresets.json .cache

# -----------------------------
# Utility Targets
# -----------------------------
CLANG_TIDY_CMD := clang-tidy
RUN_CLANG_TIDY_CMD := run-clang-tidy
CLANG_FORMAT_CMD := clang-format
CONAN_CMD := conan
GERSEMI_CMD := gersemi

LINT_COMMON_FLAGS = -p build/$(BUILD_TYPE)/ -quiet
LINT_TIDY_FLAGS = -warnings-as-errors='*'
LINT_CPUS ?= $(shell nproc)

GERSEMI_FLAGS = --list-expansion=favour-expansion --no-warn-about-unknown-commands

# Function to check for tool existence
# Usage: $(call check_tool, tool_name)
define check_tool
@if ! command -v $(1) > /dev/null 2>&1; then \
	echo "Error: Required tool '$(1)' not found."; \
	echo "Please ensure it is installed and available in your PATH."; \
	exit 1; \
fi
endef

ifdef SOURCES_TO_LINT
	FILES_TO_LINT := $(SOURCES_TO_LINT)
else ifeq ($(LINT_FILES),source)
	FILES_TO_LINT := $(shell find src tests -name '*.cpp')
else ifeq ($(LINT_FILES),header)
	FILES_TO_LINT := $(shell find src tests -name '*.hpp')
else
	FILES_TO_LINT := $(SOURCES)
endif

# Use `make lint LINT_FILES=header/source` to lint either one
lint:
	$(call check_tool,$(RUN_CLANG_TIDY_CMD))
	$(call check_tool,$(CLANG_TIDY_CMD))
	@echo "Linting with $(LINT_CPUS) cores"
	@if [ -z "$(SOURCES_TO_LINT)" ]; then \
		echo "No files to lint (LINT_FILES='$(LINT_FILES)')."; \
		exit 0; \
	fi

	@if [ "$(LINT_FILES)" = "source" ] || [ -z "$(LINT_FILES)" ]; then \
		echo "Running clang-tidy on source files..."; \
		$(RUN_CLANG_TIDY_CMD) $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS) -j $(LINT_CPUS) $(SOURCES_TO_LINT) || exit 1; \
	fi

	@if [ "$(LINT_FILES)" = "header" ] || [ -z "$(LINT_FILES)" ]; then \
		echo "Running clang-tidy on headers..."; \
		echo "$(SOURCES_TO_LINT)" | xargs -r -P $(LINT_CPUS) -n 1 $(CLANG_TIDY_CMD) $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS) || exit 1; \
	fi

	@echo "✓ Linting complete"

check-format:
	$(call check_tool,$(CLANG_FORMAT_CMD))
	$(call check_tool,$(GERSEMI_CMD))
	@echo "Checking code formatting..."
	@if $(CLANG_FORMAT_CMD) --dry-run --Werror $(SOURCES) && $(GERSEMI_CMD) --check --diff --color $(GERSEMI_FLAGS) $(SOURCES_CMAKE); then \
		echo "✓ All files are properly formatted"; \
	else \
		exit 1; \
	fi

format:
	$(call check_tool,$(CLANG_FORMAT_CMD))
	$(call check_tool,$(GERSEMI_CMD))
	@echo "Formatting code..."
	@$(CLANG_FORMAT_CMD) -i $(SOURCES)
	@$(GERSEMI_CMD) -i $(GERSEMI_FLAGS) $(SOURCES_CMAKE)
	@echo "✓ Code formatting complete"

sort-dictionary:
	@echo "Sorting dictionary..."
	@tr '[:upper:]' '[:lower:]' < .cspell_ignored | sort -f -u -o .cspell_ignored
	@echo "✓ Sorted and converted .cspell_ignored to lowercase with unique entries"

cleanup-dictionary:
	@echo "Cleaning up unused words from .cspell_ignored..."
	@.github/scripts/cleanup-cspell-ignored.sh

check-cspell-ignored:
	@echo "Checking for unused words in .cspell_ignored..."
	@.github/scripts/check-cspell-ignored.sh
	@echo "✓ Cspell ignored file check complete"

# -----------------------------
# Docker Development Environment
# -----------------------------
CONTAINER_CMD ?= docker
CI_IMAGE_NAME ?= ghcr.io/niekdomi/vhdlfmt-ci:latest

docker-dev-build:
	@echo "Building development environment with $(CONTAINER_CMD)..."
	@$(CONTAINER_CMD) compose build

docker-dev:
	@echo "Starting development shell with $(CONTAINER_CMD)..."
	@$(CONTAINER_CMD) compose run --rm dev

docker-make:
	@echo "Building project in container..."
	@$(CONTAINER_CMD) compose run --rm dev bash -c "make clean && make"

docker-test:
	@echo "Building project in container..."
	@$(CONTAINER_CMD) compose run --rm dev bash -c "make clean && make test"

docker-publish-ci:
	@echo "Building and publishing CI image to $(CI_IMAGE_NAME)..."
	@$(CONTAINER_CMD) build . -t $(CI_IMAGE_NAME) --target ci
	@$(CONTAINER_CMD) push $(CI_IMAGE_NAME)
	@echo "✓ CI image published."
