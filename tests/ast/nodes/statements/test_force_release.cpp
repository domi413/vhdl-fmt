#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ForceReleaseStatements: Force statement", "[statements][force_release]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            force <<signal .tb.dut.internal_sig : std_logic>> <= '1';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check force statement when implemented
}

TEST_CASE("ForceReleaseStatements: Release statement", "[statements][force_release]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            release <<signal .tb.dut.internal_sig : std_logic>>;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check release statement when implemented
}
