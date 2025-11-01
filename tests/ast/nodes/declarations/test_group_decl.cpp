#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("GroupDecl: Group declaration", "[declarations][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            group MyGroup is (signal clk, rst : std_logic);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check group declaration when implemented
}

TEST_CASE("GroupDecl: Group with multiple signals", "[declarations][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal clk, rst, en : std_logic;
            group ControlGroup is (signal clk, rst, en : std_logic);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check group with multiple signals when implemented
}

TEST_CASE("GroupTemplate: Group template declaration", "[declarations][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            group pin_group is (signal <>);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check group template declaration when implemented
}

TEST_CASE("GroupTemplate: Group template with multiple elements", "[declarations][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            group bus_group is (signal <>, signal <>);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check group template with multiple elements when implemented
}
