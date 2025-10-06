GRAMMARS := grammars/vhdl.g4
GENERATED := build/generated/vhdlLexer.cpp build/generated/vhdlParser.cpp
SRCS := $(shell find src tests -name '*.cpp' -o -name '*.hpp') $(GENERATED)
TARGET := build/Debug/bin/vhdl_formatter
CONAN_STAMP := build/.conan.stamp
ANTLR_STAMP := build/.antlr.stamp
BUILD_STAMP := build/.build.stamp

all: $(TARGET)

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
	@./$(TARGET) ./examples/simple.vhdl

test: $(BUILD_STAMP)
	@ctest --test-dir build/Debug --output-on-failure

# -----------------------------
# Cleanup
# -----------------------------
clean:
	@rm -rf build CMakeFiles CMakeCache.txt CMakeUserPresets.json .cache

.PHONY: all run clean conan test
