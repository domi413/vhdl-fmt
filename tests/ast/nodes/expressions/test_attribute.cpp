#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("AttributeExpr: Range attribute", "[expressions][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            process
                variable len : integer := data'range;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check range attribute when implemented
}

TEST_CASE("AttributeExpr: Length attribute", "[expressions][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type ArrayType is array (0 to 15) of integer;
            signal arr : ArrayType;
        begin
            process
                variable len : integer := arr'length;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check length attribute when implemented
}

TEST_CASE("AttributeExpr: Event attribute", "[expressions][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal clk : std_logic;
        begin
            process (clk)
            begin
                if clk'event then
                    -- do something
                end if;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check event attribute when implemented
}

TEST_CASE("AttributeExpr: High and low attributes", "[expressions][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
        begin
            process
                variable high_val : integer := data'high;
                variable low_val : integer := data'low;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check high and low attributes when implemented
}
