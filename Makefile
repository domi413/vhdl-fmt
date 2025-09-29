.PHONY: all clean run lint format check-format install

SOURCES_CPP = $(shell find parser/ -name "*.cpp" -o -name "*.hpp")
SOURCES_CMake = $(shell find . -name "CMakeLists.txt")

all: clean build

build:
	@echo "Setting up Conan profile..."
	@conan profile detect --force 2>/dev/null || true
	@echo "Installing Conan dependencies..."
	@conan install . --build=missing -s compiler.cppstd=23
	@echo "Configuring and building with Conan..."
	@cmake --preset=conan
	@cmake --build build/Release
	@echo "Creating symlink for clangd..."
	@ln -sf build/Release/compile_commands.json .

clean:
	@echo "Removing build directory..."
	@rm -rf build/

run: build
	@echo "Running the VHDL parser..."
	@./build/Release/vhdl_parser

# debug: build
# 	@echo "Running the VHDL parser in debug mode..."
# 	@gdb ./build/Release/vhdl_parser

check-format:
	@echo "Checking code formatting..."
	@if clang-format --dry-run --Werror $(SOURCES_CPP) && gersemi --check $(SOURCES_CMake); then \
		echo "✓ All files are properly formatted"; \
	else \
		exit 1; \
	fi

format:
	@echo "Formatting code..."
	@clang-format -i $(SOURCES_CPP)
	@gersemi -i $(SOURCES_CMake)
	@echo "✓ Code formatting complete"

# Flags for clang-tidy
LINT_COMMON_FLAGS = -p build/Release/
LINT_TIDY_FLAGS = --warnings-as-errors='*'

lint: build
	@echo "Running clang-tidy..."
	@clang-tidy $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS) $(SOURCES_CPP)
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
