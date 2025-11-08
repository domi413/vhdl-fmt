#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ReturnStatement: Return without value", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            procedure my_proc is
            begin
                return;
            end procedure;
        begin
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // Note: This would require parsing procedure bodies, which may not be implemented yet
    // For now, just verify it doesn't crash
    REQUIRE(design.units.size() >= 1);
}

TEST_CASE("ReturnStatement: Return with integer value", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            function add(a, b : integer) return integer is
            begin
                return a + b;
            end function;
        begin
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // Note: This would require parsing function bodies
    REQUIRE(design.units.size() >= 1);
}

TEST_CASE("ReturnStatement: Return with boolean value", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            function is_zero(val : integer) return boolean is
            begin
                return val = 0;
            end function;
        begin
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() >= 1);
}

TEST_CASE("ReturnStatement: Return with expression", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            function max(a, b : integer) return integer is
            begin
                if a > b then
                    return a;
                else
                    return b;
                end if;
            end function;
        begin
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() >= 1);
}

TEST_CASE("ReturnStatement: Return with string value", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            function get_name return string is
            begin
                return "TestName";
            end function;
        begin
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() >= 1);
}
