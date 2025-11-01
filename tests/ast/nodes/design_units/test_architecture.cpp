#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Architecture Declaration
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Basic architecture body with declarations and statements",
          "[architecture][design_unit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of MyEntity is
            signal temp : std_logic;
        begin
            temp <= '1';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->name == "RTL");
    REQUIRE(arch->entity_name == "MyEntity");
}

// -----------------------------------------------------------------------------
// Architecture with Signal Declarations
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Captures signal declarations", "[architecture][declarations]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of MyEntity is
            signal temp : std_logic;
            signal counter : integer;
        begin
            temp <= '1';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() >= 1);
}

// -----------------------------------------------------------------------------
// Architecture with Concurrent Statements
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Captures concurrent statements", "[architecture][statements]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture Behavioral of TestEntity is
            signal sig_a : std_logic;
        begin
            sig_a <= '1';
            process(clk)
            begin
                if rising_edge(clk) then
                    sig_a <= '0';
                end if;
            end process;
        end Behavioral;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->name == "Behavioral");
    REQUIRE(arch->stmts.size() >= 1);
}

// -----------------------------------------------------------------------------
// Minimal Architecture
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Minimal architecture with no declarations", "[architecture]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture Empty of MyEntity is
        begin
        end Empty;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->name == "Empty");
    REQUIRE(arch->entity_name == "MyEntity");
}

// -----------------------------------------------------------------------------
// Architecture without End Label
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Can omit end label", "[architecture]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of TestEntity is
        begin
            null;
        end;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->name == "RTL");
}

// -----------------------------------------------------------------------------
// Multiple Architectures
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Multiple architecture styles for same entity", "[architecture]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Counter is
        begin
        end RTL;

        architecture Behavioral of Counter is
        begin
        end Behavioral;

        architecture Structural of Counter is
        begin
        end Structural;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 3);

    auto *arch1 = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch1 != nullptr);
    REQUIRE(arch1->name == "RTL");
    REQUIRE(arch1->entity_name == "Counter");

    auto *arch2 = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch2 != nullptr);
    REQUIRE(arch2->name == "Behavioral");
    REQUIRE(arch2->entity_name == "Counter");

    auto *arch3 = std::get_if<ast::Architecture>(&design.units[2]);
    REQUIRE(arch3 != nullptr);
    REQUIRE(arch3->name == "Structural");
    REQUIRE(arch3->entity_name == "Counter");
}

// -----------------------------------------------------------------------------
// Architecture with Complex Declarations
// -----------------------------------------------------------------------------
TEST_CASE("Architecture: Handles multiple declaration types", "[architecture][declarations]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of ComplexEntity is
            signal sig1 : std_logic;
            signal sig2 : std_logic_vector(7 downto 0);
            constant MAX_COUNT : integer := 255;
        begin
            sig1 <= '0';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[0]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->name == "RTL");
    REQUIRE(arch->decls.size() >= 1);
}
