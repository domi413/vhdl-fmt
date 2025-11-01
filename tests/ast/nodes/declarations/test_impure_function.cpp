#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ImpureFunction: Impure function declaration", "[declarations][impure_function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            impure function GetCount return integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check impure function when implemented
}

TEST_CASE("ImpureFunction: Impure function with parameters", "[declarations][impure_function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            impure function Calculate(a, b : integer) return integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check impure function with params when implemented
}

TEST_CASE("ImpureFunction: Impure function body", "[declarations][impure_function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            impure function GetRandom return integer is
            begin
                return 42; -- placeholder
            end function GetRandom;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check impure function body when implemented
}

TEST_CASE("ImpureFunction: Impure function in protected type", "[declarations][impure_function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            protected type Counter is
                impure function get_value return integer;
            end protected type;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check impure function in protected type when implemented
}
