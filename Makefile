# Default preset, override with `make BUILD_TYPE=Release`
BUILD_TYPE ?= Debug
CMAKE_PRESET := conan-$(shell echo $(BUILD_TYPE) | tr A-Z a-z)

TARGET := build/$(BUILD_TYPE)/bin/vhdl_formatter
CONAN_STAMP := build/.conan.$(BUILD_TYPE).stamp
BUILD_STAMP := build/.build.$(BUILD_TYPE).stamp

SRCS := $(shell find src tests -name '*.cpp' -o -name '*.hpp')
SRCS_CMAKE := $(shell find src tests -name 'CMakeLists.txt')

all: $(BUILD_STAMP)

$(BUILD_STAMP): $(SRCS) $(SRCS_CMAKE) $(CONAN_STAMP)
	@echo "Building project ($(BUILD_TYPE))..."
	@cmake --preset $(CMAKE_PRESET)
	@cmake --build --preset $(CMAKE_PRESET)
	@touch $@
	@echo "Build complete."

$(CONAN_STAMP): conanfile.txt
	@echo "Running Conan ($(BUILD_TYPE))..."
	@conan install . \
		-pr=clang.profile \
		--build=missing \
		-s build_type=$(BUILD_TYPE)
	@touch $@

conan: $(CONAN_STAMP)

run: $(BUILD_STAMP)
	@./$(TARGET) ./tests/data/simple.vhdl

test: $(BUILD_STAMP)
	@ctest --preset $(CMAKE_PRESET)

# -----------------------------
# Build rules
# -----------------------------
$(BUILD_STAMP): $(SRCS) CMakeLists.txt $(CONAN_STAMP) $(ANTLR_STAMP)
	@echo "Building project..."
	@cmake --preset conan-debug
	@cmake --build --preset conan-debug
	@touch $@
	@echo "Build complete."

$(TARGET): $(BUILD_STAMP)

$(ANTLR_STAMP): $(GRAMMARS) $(CONAN_STAMP)
	@echo "Generating ANTLR4 sources..."
	@bash -c '. build/Debug/generators/conanrun.sh && \
	cd grammars && \
	antlr4 -Dlanguage=Cpp vhdl.g4 -lib . -o ../build/generated/ -no-listener -visitor'
	@touch $@
	@echo "Generation complete."

$(GENERATED): $(ANTLR_STAMP)

$(CONAN_STAMP): conanfile.txt
	@echo "Running Conan..."
	@CC=clang CXX=clang++ conan install . \
		--build=missing \
		-pr:h=conan_profiles/ninja_debug.profile \
		-s:h build_type=Debug
	@touch $(CONAN_STAMP)

conan: $(CONAN_STAMP)

# -----------------------------
# Run & Test targets
# -----------------------------
run: $(BUILD_STAMP)
	@./$(TARGET) ./tests/data/simple.vhdl

test: $(BUILD_STAMP)
	@ctest --test-dir build/Debug --output-on-failure

# -----------------------------
# Cleanup
# -----------------------------
clean:
	@rm -rf build CMakeFiles CMakeCache.txt CMakeUserPresets.json .cache

.PHONY: all run clean conan test

# -----------------------------
# Utility targets
# -----------------------------
LINT_COMMON_FLAGS = -p build/$(BUILD_TYPE)/ -quiet
LINT_TIDY_FLAGS = --warnings-as-errors='*'
LINT_CPUS ?= $(shell nproc)

lint:
	@echo "Running clang-tidy on source files..."
	@CLANG_TIDY_EXTRA_ARGS="$(LINT_TIDY_FLAGS)" \
	run-clang-tidy $(LINT_COMMON_FLAGS) -j $(LINT_CPUS) $(SRCS)

	@echo "Running clang-tidy on headers..."
	@find src tests \( -path '*/build/*' -o -path '*/generated/*' -o -path '*/generators/*' -o -path '*/external/*' \) -prune \
		-o -type f \( -name '*.hpp' -o -name '*.h' \) -print | \
		xargs -r -P $(LINT_CPUS) -n 1 clang-tidy $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS)

	@echo "✓ Linting complete"


check-format:
	@echo "Checking code formatting..."
	@if clang-format --dry-run --Werror $(SRCS) && gersemi --check $(SRCS_CMAKE); then \
		echo "✓ All files are properly formatted"; \
	else \
		exit 1; \
	fi

format:
	@echo "Formatting code..."
	@clang-format -i $(SRCS)
	@gersemi -i $(SRCS_CMAKE)
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

.PHONY: all run clean conan test
