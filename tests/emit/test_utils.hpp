#ifndef EMIT_TEST_UTILS_HPP
#define EMIT_TEST_UTILS_HPP

#include "common/config.hpp"

namespace emit::test {

// Default config for tests - uses indent_size of 2 to match existing test expectations
inline auto defaultConfig() -> common::Config
{
    common::Config config;
    config.line_config.indent_size = 2;
    return config;
}

} // namespace emit::test

#endif // EMIT_TEST_UTILS_HPP
