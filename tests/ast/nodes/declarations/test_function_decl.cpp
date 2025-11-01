#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("FunctionDecl: Function prototype", "[declarations][function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            function Add(a, b : integer) return integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function declaration when implemented
}

TEST_CASE("FunctionBody: Function implementation", "[declarations][function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end function Add;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function body when implemented
}

TEST_CASE("FunctionDecl: Function with parameters", "[declarations][function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            function Multiply(a : integer; b : integer) return integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function with params when implemented
}

TEST_CASE("ImpureFunction: Impure function", "[declarations][function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            impure function GetCount return integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check impure function when implemented
}
