#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ConditionalExpr: Simple when-else", "[expressions][conditional]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := value when condition else other_value;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check conditional expression when implemented
}

TEST_CASE("ConditionalExpr: Multiple conditions", "[expressions][conditional]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := a when x = 1 else b when x = 2 else c;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple conditions when implemented
}

TEST_CASE("ConditionalExpr: In signal assignment", "[expressions][conditional]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal output : std_logic;
            signal sel : std_logic;
        begin
            output <= '1' when sel = '1' else '0';
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check conditional in signal assignment when implemented
}

TEST_CASE("ConditionalExpr: Nested conditionals", "[expressions][conditional]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := (a when cond1 else b) when cond2 else c;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check nested conditionals when implemented
}
