#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ContextClause: Simple context declaration", "[clauses][context_clause]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context my_context is
            library ieee;
            use ieee.std_logic_1164.all;
        end context my_context;
    )";

    // Context declarations not yet implemented - just verify parsing succeeds
    const auto design = builder::buildFromString(VHDL_FILE);
    // Note: context declarations are not yet stored in design.units
    REQUIRE(design.units.size() == 0);
}

TEST_CASE("ContextClause: Context with multiple libraries and uses", "[clauses][context_clause]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context complex_context is
            library ieee, std;
            use ieee.std_logic_1164.all;
            use std.textio.all;
        end context complex_context;
    )";

    // Context declarations not yet implemented - just verify parsing succeeds
    const auto design = builder::buildFromString(VHDL_FILE);
    // Note: context declarations are not yet stored in design.units
    REQUIRE(design.units.size() == 0);
}
