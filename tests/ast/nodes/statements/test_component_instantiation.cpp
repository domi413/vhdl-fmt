#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("ComponentInstantiation: Simple component instantiation",
          "[statements][component_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            component MyComponent is
                port (input : in std_logic; output : out std_logic);
            end component;
        begin
            U1 : MyComponent port map (input => in_sig, output => out_sig);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check component instantiation when implemented
}

TEST_CASE("ComponentInstantiation: Instantiation with generic map",
          "[statements][component_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
            component MyComponent is
                generic (width : integer := 8);
                port (input : in std_logic_vector(width-1 downto 0); output : out std_logic_vector(width-1 downto 0));
            end component;
        begin
            U1 : MyComponent generic map (width => 16) port map (input => in_sig, output => out_sig);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check component instantiation when implemented
}
