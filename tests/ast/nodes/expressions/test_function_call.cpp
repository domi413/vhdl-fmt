#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("makeFunctionCallOrIndexedNamePart: Simple function call", "[expressions][function_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := Add(a, b);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function call when implemented
}

TEST_CASE("makeFunctionCallOrIndexedNamePart: Function call with conversion", "[expressions][function_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := to_integer(value);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function call with conversion when implemented
}

TEST_CASE("makeFunctionCallOrIndexedNamePart: Function call with no parameters", "[expressions][function_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := GetRandom();
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function call with no params when implemented
}

TEST_CASE("makeFunctionCallOrIndexedNamePart: Nested function calls", "[expressions][function_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := Add(Multiply(x, y), z);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check nested function calls when implemented
}
