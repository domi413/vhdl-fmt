#ifndef TESTS_COMMON_BENCHMARK_UTILS_HPP
#define TESTS_COMMON_BENCHMARK_UTILS_HPP

#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <memory>
#include <string_view>

namespace antlr4 {
class ANTLRInputStream;
class CommonTokenStream;
class ParserRuleContext;
class Token;
} // namespace antlr4

namespace utils {

extern const std::string_view STRESS_TEST_VHDL;

/// @brief A helper to manage ANTLR object lifetime for tests/benchmarks.
struct ParsingContext
{
    std::unique_ptr<antlr4::ANTLRInputStream> input;
    std::unique_ptr<vhdlLexer> lexer;
    std::unique_ptr<antlr4::CommonTokenStream> tokens;
    std::unique_ptr<vhdlParser> parser;

    // The resulting CST
    vhdlParser::Design_fileContext *tree{};

    explicit ParsingContext(std::string_view source);

    // Parse using the specified mode (SLL or LL)
    void parse(bool use_sll);
};

/// @brief Replicates the exact raw parsing logic (Lexer+Parser creation)
/// used in benchmarks to measure "Parser Construction + Execution".
/// @return The parser context (discarded immediately in benchmarks)
auto benchmarkRawParse(std::string_view source, bool use_sll) -> vhdlParser::Design_fileContext *;

} // namespace utils

#endif // TESTS_COMMON_BENCHMARK_UTILS_HPP
