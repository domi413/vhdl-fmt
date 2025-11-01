#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ConditionalAssign: Simple conditional assignment", "[statements][conditional_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            output <= '1' when condition else '0';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check conditional assign statement when implemented
}

TEST_CASE("ConditionalAssign: Conditional assignment with multiple conditions",
          "[statements][conditional_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            output <= '1' when sel = "00" else
                     '0' when sel = "01" else
                     'Z';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check conditional assign statement when implemented
}
