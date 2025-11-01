#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("CaseGenerate: Simple case generate", "[statements][case_generate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            gen_case: case selector generate
                when "00" =>
                    signal sig1 : std_logic;
                when "01" =>
                    signal sig2 : std_logic;
            end generate;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check case generate when implemented
}

TEST_CASE("CaseGenerate: Case generate with others", "[statements][case_generate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            gen_case: case selector generate
                when "00" =>
                    signal sig1 : std_logic;
                when "01" =>
                    signal sig2 : std_logic;
                when others =>
                    signal sig3 : std_logic;
            end generate;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check case generate when implemented
}
