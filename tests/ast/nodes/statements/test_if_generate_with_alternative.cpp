#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("IfGenerateWithAlternative: If-else generate", "[statements][if_generate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            gen_if: if condition generate
                signal sig1 : std_logic;
            else generate
                signal sig2 : std_logic;
            end generate;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check if generate with alternative when implemented
}

TEST_CASE("IfGenerateWithAlternative: If-elsif-else generate", "[statements][if_generate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            gen_if: if cond1 generate
                signal sig1 : std_logic;
            elsif cond2 generate
                signal sig2 : std_logic;
            else generate
                signal sig3 : std_logic;
            end generate;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check if generate with alternative when implemented
}
