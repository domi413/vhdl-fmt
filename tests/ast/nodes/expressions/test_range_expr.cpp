#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("RangeExpr: To range", "[expressions][range]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(0 to 10);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check to range when implemented
}

TEST_CASE("RangeExpr: Downto range", "[expressions][range]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(10 downto 0);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check downto range when implemented
}

TEST_CASE("RangeExpr: Range in subtype", "[expressions][range]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            subtype MyRange is integer range 0 to 100;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check range in subtype when implemented
}

TEST_CASE("RangeExpr: Range with expressions", "[expressions][range]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant WIDTH : integer := 8;
            signal data : std_logic_vector(0 to WIDTH-1);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check range with expressions when implemented
}
