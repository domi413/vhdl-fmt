#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("OpenExpr: Open in port map", "[expressions][open]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (input : in std_logic; output : out std_logic);
        end E;
        architecture A of E is
            component Comp is
                port (in_sig : in std_logic; out_sig : out std_logic);
            end component;
        begin
            inst : Comp port map (in_sig => input, out_sig => open);
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check open in port map when implemented
}

TEST_CASE("OpenExpr: Open in generic map", "[expressions][open]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (WIDTH : integer := 8);
        end E;
        architecture A of E is
            component Comp is
                generic (W : integer := 8; DEPTH : integer := 16);
                port (data : in std_logic_vector(W-1 downto 0));
            end component;
        begin
            inst : Comp generic map (W => WIDTH, DEPTH => open) port map (data => (others => '0'));
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check open in generic map when implemented
}

TEST_CASE("OpenExpr: Multiple opens in port map", "[expressions][open]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b : in std_logic; x, y : out std_logic);
        end E;
        architecture A of E is
            component Comp is
                port (in1, in2 : in std_logic; out1, out2 : out std_logic);
            end component;
        begin
            inst : Comp port map (in1 => a, in2 => b, out1 => open, out2 => open);
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple opens when implemented
}

TEST_CASE("OpenExpr: Open in function call", "[expressions][open]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := func(a, open);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check open in function call when implemented
}
