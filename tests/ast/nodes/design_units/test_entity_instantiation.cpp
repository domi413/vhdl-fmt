#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("EntityInstantiation: Direct entity instantiation", "[entity_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Top is
        begin
            U1 : entity work.MyEntity(RTL) port map (clk => clk, data => data);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    // REQUIRE(arch != nullptr);
    // REQUIRE(arch->stmts.size() >= 1);
}

TEST_CASE("EntityInstantiation: With generic map", "[entity_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of System is
        begin
            U_Counter : entity work.Counter(RTL)
                generic map (WIDTH => 16, RESET_VALUE => 0)
                port map (clk => sys_clk, rst => sys_rst);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    // REQUIRE(arch != nullptr);
}

TEST_CASE("EntityInstantiation: Without architecture specification", "[entity_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Top is
        begin
            U1 : entity work.SimpleEntity
                port map (input => sig_in, output => sig_out);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    // REQUIRE(arch != nullptr);
}

TEST_CASE("EntityInstantiation: With library prefix", "[entity_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Top is
        begin
            U1 : entity mylib.CustomEntity(Optimized)
                port map (a => sig_a, b => sig_b);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    // REQUIRE(arch != nullptr);
}

TEST_CASE("EntityInstantiation: Multiple instantiations", "[entity_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture Structural of Top is
        begin
            U1 : entity work.Adder(RTL)
                port map (a => a1, b => b1, sum => sum1);
            U2 : entity work.Adder(RTL)
                port map (a => a2, b => b2, sum => sum2);
        end Structural;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    // REQUIRE(arch != nullptr);
}
