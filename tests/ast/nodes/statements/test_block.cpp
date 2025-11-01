#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("Block: Simple block statement", "[statements][block]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            my_block : block
            begin
                signal temp : std_logic;
            begin
                temp <= '1';
            end block my_block;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check block statement when implemented
}

TEST_CASE("Block: Block with guard", "[statements][block]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            guarded_block : block (enable = '1')
            begin
                signal temp : std_logic;
            begin
                temp <= guarded input;
            end block guarded_block;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check block statement when implemented
}
