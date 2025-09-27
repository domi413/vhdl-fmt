.PHONY: all clean run lint format check-format

SOURCES_CPP = $(shell find FOLDERNAME/ tests/ -name "*.cpp" -o -name "*.hpp")
SOURCES_CMake = $(shell find FOLDERNAME/ tests/ -name "*CMakeLists.txt")

all: clean build

build:
	@echo "Configuring and building with Ninja..."
	@cmake --preset=default -GNinja
	@cmake --build build

clean:
	@echo "Removing build directory..."
	@rm -rf build/

run: build
	@echo "Running the project..."
	# THIS PATH SHOULD BE DEFINED WITH A CONSTANT 

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
LINT_COMMON_FLAGS = -p build/
LINT_TIDY_FLAGS = --warnings-as-errors='*'

lint: build
	@echo "Running clang-tidy..."
	@clang-tidy $(LINT_COMMON_FLAGS) $(LINT_TIDY_FLAGS) $(SOURCES_CPP)
	@echo "✓ Linting complete"
