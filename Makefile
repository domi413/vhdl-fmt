GRAMMARS := grammars/vhdl.g4
GENERATED := build/generated/vhdlLexer.cpp build/generated/vhdlParser.cpp
SRCS := $(shell find src tests -name '*.cpp' -o -name '*.hpp')
SRCS_CMAKE := $(shell find -name 'CMakeLists.txt')
TARGET := build/Debug/bin/vhdl_formatter
CONAN_STAMP := build/.conan.stamp
ANTLR_STAMP := build/.antlr.stamp
BUILD_STAMP := build/.build.stamp

# Flags for clang-tidy
LINT_COMMON_FLAGS = -p build/Debug/generators/
LINT_TIDY_FLAGS = --warnings-as-errors='*'

all: $(TARGET)

print-SRCS:
	@echo $(SRCS)

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
	@./$(TARGET) ./tests/examples/simple.vhdl

test: $(BUILD_STAMP)
	@ctest --test-dir build/Debug --output-on-failure

# -----------------------------
# Cleanup
# -----------------------------
clean:
	@rm -rf build CMakeFiles CMakeCache.txt CMakeUserPresets.json .cache

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

lint: build
	@echo "Running clang-tidy..."
	@clang-tidy $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS) $(SRCS)
	@echo "✓ Linting complete"

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
