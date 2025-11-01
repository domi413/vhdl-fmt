#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("QualifiedExpr: Qualified expression with type", "[expressions][qualified]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : std_logic_vector(7 downto 0) := std_logic_vector'(value);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check qualified expression when implemented
}

TEST_CASE("QualifiedExpr: Qualified with subtype", "[expressions][qualified]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            subtype SmallInt is integer range 0 to 255;
        begin
            process
                variable val : SmallInt := SmallInt'(42);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check qualified with subtype when implemented
}

TEST_CASE("QualifiedExpr: Qualified in assignment", "[expressions][qualified]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable data : std_logic_vector(3 downto 0);
            begin
                data := std_logic_vector'("1010");
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check qualified in assignment when implemented
}

TEST_CASE("QualifiedExpr: Qualified with aggregate", "[expressions][qualified]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type MyRecord is record
                a : integer;
                b : std_logic;
            end record;
        begin
            process
                variable rec : MyRecord := MyRecord'(a => 1, b => '0');
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check qualified with aggregate when implemented
}
