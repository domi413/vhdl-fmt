#include "ast/nodes/expressions.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>
TEST_CASE("ReductionOps: AND reduction", "[expressions][reduction]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            process
                variable result : std_logic := and data;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check AND reduction when implemented
}

TEST_CASE("ReductionOps: OR reduction", "[expressions][reduction]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            process
                variable result : std_logic := or data;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check OR reduction when implemented
}

TEST_CASE("ReductionOps: XOR reduction", "[expressions][reduction]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            process
                variable result : std_logic := xor data;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check XOR reduction when implemented
}

TEST_CASE("ReductionOps: NAND reduction", "[expressions][reduction]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            process
                variable result : std_logic := nand data;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check NAND reduction when implemented
}
