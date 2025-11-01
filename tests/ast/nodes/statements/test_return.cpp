#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Return: Simple return statement", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package Test_Pkg is
            function Simple_Func return integer;
        end Test_Pkg;

        package body Test_Pkg is
            function Simple_Func return integer is
            begin
                return 42;
            end Simple_Func;
        end Test_Pkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check return statement when implemented
}

TEST_CASE("Return: Return with expression", "[statements][return]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package Test_Pkg is
            function Add(a, b : integer) return integer;
        end Test_Pkg;

        package body Test_Pkg is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end Add;
        end Test_Pkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check return statement when implemented
}
