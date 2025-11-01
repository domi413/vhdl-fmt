#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("Next: Simple next statement", "[statements][next]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    next;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check next statement when implemented
}

TEST_CASE("Next: Next with condition", "[statements][next]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    next when i = 5;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check next statement when implemented
}
