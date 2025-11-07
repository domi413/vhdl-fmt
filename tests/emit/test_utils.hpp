#ifndef EMIT_TEST_UTILS_HPP
#define EMIT_TEST_UTILS_HPP

#include "common/config.hpp"

namespace emit::test {

// Default config for tests - uses indent_size of 2 to match existing test expectations
inline auto defaultConfig() -> common::Config
{
    constexpr int TEST_INDENT_SIZE = 2;
    constexpr int TEST_LINE_LENGTH = 80;

    common::Config config;
    config.line_config.indent_size = TEST_INDENT_SIZE;
    config.line_config.line_length = TEST_LINE_LENGTH;
    return config;
}

} // namespace emit::test

#endif // EMIT_TEST_UTILS_HPP
