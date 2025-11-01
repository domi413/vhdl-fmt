#include "ast/nodes/expressions.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>
TEST_CASE("TypeConversion: Integer conversion", "[expressions][type_conversion]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := integer(value);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check integer conversion when implemented
}

TEST_CASE("TypeConversion: Std_logic_vector conversion", "[expressions][type_conversion]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : std_logic_vector(7 downto 0) := std_logic_vector(data);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check std_logic_vector conversion when implemented
}

TEST_CASE("TypeConversion: Conversion in port map", "[expressions][type_conversion]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (input : in integer);
        end E;
        architecture A of E is
            component Comp is
                port (in_sig : in std_logic_vector(7 downto 0));
            end component;
        begin
            inst : Comp port map (in_sig => std_logic_vector(input));
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check conversion in port map when implemented
}

TEST_CASE("TypeConversion: Nested conversions", "[expressions][type_conversion]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : std_logic := std_logic(integer('1'));
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check nested conversions when implemented
}
