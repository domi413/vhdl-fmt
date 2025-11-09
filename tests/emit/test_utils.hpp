#ifndef EMIT_TEST_UTILS_HPP
#define EMIT_TEST_UTILS_HPP

#include "common/config.hpp"
#include "emit/pretty_printer.hpp"

#include <string>

namespace emit::test {

// Default config for tests - uses indent_size of 2 to match existing test expectations
constexpr auto defaultConfig() -> common::Config
{
    constexpr int TEST_INDENT_SIZE = 2;
    constexpr int TEST_LINE_LENGTH = 80;

    return common::Config{
        .line_config = { .line_length = TEST_LINE_LENGTH, .indent_size = TEST_INDENT_SIZE },
        .port_map = { .align_signals = false },
    };
}

// Helper to render an AST node with default config
template<typename T>
auto render(const T &node) -> std::string
{
    emit::PrettyPrinter printer{ defaultConfig() };
    const auto doc = printer(node);
    return doc.render(defaultConfig());
}

// Helper to render an AST node with custom config
template<typename T>
auto render(const T &node, const common::Config &config) -> std::string
{
    emit::PrettyPrinter printer{ config };
    const auto doc = printer(node);
    return doc.render(config);
}

} // namespace emit::test

#endif // EMIT_TEST_UTILS_HPP
