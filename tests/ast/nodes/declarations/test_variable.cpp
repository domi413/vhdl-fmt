#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("makeVariableDecl: Variable in process", "[declarations][variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable count : integer := 0;
            begin
                count := count + 1;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check variable declaration when implemented
}

TEST_CASE("makeVariableDecl: Variable in function", "[declarations][variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            function Func return integer is
                variable temp : integer := 5;
            begin
                return temp;
            end function Func;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check variable declaration in function when implemented
}

TEST_CASE("makeVariableDecl: Multiple variables in process", "[declarations][variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable a, b, c : integer := 0;
            begin
                a := 1;
                b := 2;
                c := 3;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple variable declarations when implemented
}

TEST_CASE("makeVariableDecl: Variable without initialization", "[declarations][variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable count : integer;
            begin
                count := 10;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check variable without init when implemented
}
