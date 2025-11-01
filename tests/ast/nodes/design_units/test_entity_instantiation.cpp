#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Entity Instantiation
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Direct entity instantiation without components",
          "[entity_instantiation][instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Top is
        begin
            U1 : entity work.MyEntity(RTL) port map (clk => clk, data => data);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // REQUIRE(arch->stmts.size() >= 1);
    // Verify that statement is an entity instantiation
    // Verify label is "U1"
    // Verify entity name is "MyEntity" from library "work"
    // Verify architecture name is "RTL"
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Port Map
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Captures port map associations",
          "[entity_instantiation][port_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture Structural of TestBench is
        begin
            DUT : entity work.Counter(Behavioral)
                port map (
                    clk => clock,
                    rst => reset,
                    count => output_count
                );
        end Structural;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify label is "DUT"
    // Verify port map has 3 associations
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Handles generic map parameters",
          "[entity_instantiation][generic_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of System is
        begin
            U_Counter : entity work.Counter(RTL)
                generic map (
                    WIDTH => 16,
                    RESET_VALUE => 0
                )
                port map (
                    clk => sys_clk,
                    rst => sys_rst
                );
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify generic map has 2 parameters
    // Verify port map associations
}

// -----------------------------------------------------------------------------
// Entity Instantiation without Architecture
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Can omit architecture specification",
          "[entity_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Top is
        begin
            U1 : entity work.SimpleEntity
                port map (input => sig_in, output => sig_out);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify no architecture name is specified
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Library Prefix
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Supports different library names",
          "[entity_instantiation][library][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Top is
        begin
            U1 : entity mylib.CustomEntity(Optimized)
                port map (a => sig_a, b => sig_b);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify library is "mylib"
    // Verify entity is "CustomEntity"
    // Verify architecture is "Optimized"
}

// -----------------------------------------------------------------------------
// Multiple Entity Instantiations
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Architecture with multiple instantiations",
          "[entity_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture Structural of Top is
        begin
            U1 : entity work.Adder(RTL)
                port map (a => a1, b => b1, sum => sum1);

            U2 : entity work.Adder(RTL)
                port map (a => a2, b => b2, sum => sum2);

            U3 : entity work.Multiplier(Fast)
                port map (x => x1, y => y1, result => prod1);
        end Structural;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // REQUIRE(arch->stmts.size() >= 3);
    // Verify all three instantiation labels
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Positional Port Map
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Supports positional port map",
          "[entity_instantiation][port_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
        begin
            U1 : entity work.SimpleGate(RTL)
                port map (a, b, c);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify positional (non-named) associations
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Named Port Map
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Supports named port map associations",
          "[entity_instantiation][port_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
        begin
            U1 : entity work.Module(Behavioral)
                port map (
                    clk => system_clock,
                    data_in => input_data,
                    data_out => output_data,
                    enable => enable_signal
                );
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify named associations
    // Verify 4 port mappings
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Open Ports
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Handles open port connections",
          "[entity_instantiation][port_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
        begin
            U1 : entity work.OptionalPorts(RTL)
                port map (
                    required_in => sig_in,
                    required_out => sig_out,
                    optional_in => open,
                    optional_out => open
                );
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify "open" keyword usage in port map
}

// -----------------------------------------------------------------------------
// Entity Instantiation without Port Map
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Entity with no ports needs no port map",
          "[entity_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
        begin
            U1 : entity work.NoPortEntity(RTL);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify instantiation without port map clause
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Generic Map Expression
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Supports expressions in generic map",
          "[entity_instantiation][generic_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
            constant BASE_WIDTH : integer := 8;
        begin
            U1 : entity work.ConfigurableEntity(RTL)
                generic map (
                    WIDTH => BASE_WIDTH * 2,
                    DEPTH => 2**10,
                    ENABLE => true
                )
                port map (clk => clk);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify expression evaluation in generic map
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Default Generic Values
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Can use default generic values",
          "[entity_instantiation][generic_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
        begin
            U1 : entity work.DefaultGenericEntity(RTL)
                port map (clk => clk, data => data);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify instantiation without generic map (using defaults)
}

// -----------------------------------------------------------------------------
// Entity Instantiation vs Component Instantiation
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Distinguishes from component instantiation",
          "[entity_instantiation][component][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture Mixed of Test is
            component MyComponent
                port (a : in std_logic);
            end component;
        begin
            -- Component instantiation
            U1 : MyComponent port map (a => sig_a);

            -- Entity instantiation
            U2 : entity work.MyEntity(RTL) port map (a => sig_a);
        end Mixed;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // REQUIRE(arch->stmts.size() >= 2);
    // Verify first is component instantiation
    // Verify second is entity instantiation
}

// -----------------------------------------------------------------------------
// Entity Instantiation with Complex Port Map
// -----------------------------------------------------------------------------
TEST_CASE("EntityInstantiation: Handles complex port map expressions",
          "[entity_instantiation][port_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Test is
            signal vec : std_logic_vector(15 downto 0);
        begin
            U1 : entity work.SliceEntity(RTL)
                port map (
                    upper_byte => vec(15 downto 8),
                    lower_byte => vec(7 downto 0),
                    bit_0 => vec(0),
                    constant_in => '1'
                );
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);

    // TODO: Uncomment when entity instantiation is implemented
    // Verify slice expressions and constant values in port map
}
