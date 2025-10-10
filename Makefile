.PHONY: all run clean conan test

# Default preset, override with `make BUILD_TYPE=Release`
BUILD_TYPE ?= Debug
CMAKE_PRESET := conan-$(shell echo $(BUILD_TYPE) | tr A-Z a-z)

TARGET := build/$(BUILD_TYPE)/bin/vhdl_formatter
CONAN_STAMP := build/.conan.$(BUILD_TYPE).stamp
BUILD_STAMP := build/.build.$(BUILD_TYPE).stamp

SOURCES := $(shell find src tests -name '*.cpp' -o -name '*.hpp')
SOURCES_CMAKE := $(shell find src tests . -maxdepth 1 -name 'CMakeLists.txt')

ifeq ($(wildcard venv/bin),venv/bin)
	VENV_BIN := venv/bin/
else
	VENV_BIN :=
endif

all: $(BUILD_STAMP)

$(BUILD_STAMP): $(SOURCES) $(SOURCES_CMAKE) $(CONAN_STAMP)
	@echo "Building project ($(BUILD_TYPE))..."
	@cmake --preset $(CMAKE_PRESET)
	@cmake --build --preset $(CMAKE_PRESET)
	@touch $@
	@echo "Build complete."

$(CONAN_STAMP): conanfile.txt
	@echo "Running Conan ($(BUILD_TYPE))..."
	@$(VENV_BIN)conan install . \
		-pr=clang.profile \
		--build=missing \
		-s build_type=$(BUILD_TYPE)
	@touch $@

conan: $(CONAN_STAMP)

run: $(BUILD_STAMP)
	@./$(TARGET) ./tests/data/simple.vhdl

test: $(BUILD_STAMP)
	@ctest --preset $(CMAKE_PRESET)

clean:
	@rm -rf build CMakeFiles CMakeCache.txt CMakeUserPresets.json .cache

# -----------------------------
# Utility targets
# -----------------------------
LINT_COMMON_FLAGS = -p build/$(BUILD_TYPE)/ -quiet
LINT_TIDY_FLAGS = --warnings-as-errors='*'
LINT_CPUS ?= $(shell nproc)

lint:
	@echo "Running clang-tidy on source files..."
	@CLANG_TIDY_EXTRA_ARGS="$(LINT_TIDY_FLAGS)" \
	run-clang-tidy $(LINT_COMMON_FLAGS) -j $(LINT_CPUS) $(SOURCES)

	@echo "Running clang-tidy on headers..."
	@find src tests \( -path '*/build/*' -o -path '*/generated/*' -o -path '*/generators/*' -o -path '*/external/*' \) -prune \
		-o -type f \( -name '*.hpp' -o -name '*.h' \) -print | \
		xargs -r -P $(LINT_CPUS) -n 1 clang-tidy $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS)

	@echo "✓ Linting complete"


GERMESI_FLAGS = --list-expansion=favour-expansion --no-warn-about-unknown-commands
check-format:
	@echo "Checking code formatting..."
	@if clang-format --dry-run --Werror $(SOURCES) && $(VENV_BIN)gersemi --check $(GERMESI_FLAGS) $(SOURCES_CMAKE); then \
		echo "✓ All files are properly formatted"; \
	else \
		exit 1; \
	fi

format:
	@echo "Formatting code..."
	@clang-format -i $(SOURCES)
	@$(VENV_BIN)gersemi -i $(GERMESI_FLAGS) $(SOURCES_CMAKE)
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
