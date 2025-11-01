#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Entity Declaration
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Basic entity declaration with ports and generics", "[entity][design_unit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MyEntity is
            generic (WIDTH : integer := 8);
            port (clk : in std_logic; data : out std_logic_vector(7 downto 0));
        end MyEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->name == "MyEntity");
    REQUIRE(entity->end_label.has_value());
    REQUIRE(entity->end_label.value() == "MyEntity");
}

// -----------------------------------------------------------------------------
// Entity with Generic Clause
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Captures generic parameters", "[entity][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MyEntity is
            generic (WIDTH : integer := 8);
            port (clk : in std_logic);
        end MyEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    const auto &gen = entity->generic_clause.generics[0];
    REQUIRE(gen.names.size() == 1);
    REQUIRE(gen.names[0] == "WIDTH");
}

// -----------------------------------------------------------------------------
// Entity with Port Clause
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Captures port declarations", "[entity][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MyEntity is
            port (
                clk : in std_logic;
                data : out std_logic_vector(7 downto 0)
            );
        end MyEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 2);

    const auto &clk_port = entity->port_clause.ports[0];
    REQUIRE(clk_port.names.size() == 1);
    REQUIRE(clk_port.names[0] == "clk");

    const auto &data_port = entity->port_clause.ports[1];
    REQUIRE(data_port.names.size() == 1);
    REQUIRE(data_port.names[0] == "data");
}

// -----------------------------------------------------------------------------
// Entity with Multiple Generics
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Handles multiple generic parameters", "[entity][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Counter is
            generic (
                WIDTH : integer := 8;
                RESET_VAL : integer := 0;
                ENABLE_ASYNC : boolean := false
            );
            port (clk : in std_logic);
        end Counter;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->name == "Counter");
    REQUIRE(entity->generic_clause.generics.size() == 3);

    REQUIRE(entity->generic_clause.generics[0].names.size() == 1);
    REQUIRE(entity->generic_clause.generics[0].names[0] == "WIDTH");
    REQUIRE(entity->generic_clause.generics[1].names.size() == 1);
    REQUIRE(entity->generic_clause.generics[1].names[0] == "RESET_VAL");
    REQUIRE(entity->generic_clause.generics[2].names.size() == 1);
    REQUIRE(entity->generic_clause.generics[2].names[0] == "ENABLE_ASYNC");
}

// -----------------------------------------------------------------------------
// Entity without End Label
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Can omit end label", "[entity]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity SimpleEntity is
            port (clk : in std_logic);
        end;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->name == "SimpleEntity");
}

// -----------------------------------------------------------------------------
// Minimal Entity
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Minimal entity without ports or generics", "[entity]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MinimalEntity is
        end MinimalEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->name == "MinimalEntity");
    REQUIRE(entity->generic_clause.generics.empty());
    REQUIRE(entity->port_clause.ports.empty());
}

// -----------------------------------------------------------------------------
// Entity with Only Generics
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Entity with generics but no ports", "[entity][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity ConfigEntity is
            generic (
                CONFIG_VAL : integer := 42
            );
        end ConfigEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);
    REQUIRE(entity->port_clause.ports.empty());
}

// -----------------------------------------------------------------------------
// Entity with Only Ports
// -----------------------------------------------------------------------------
TEST_CASE("Entity: Entity with ports but no generics", "[entity][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity SimpleIO is
            port (
                input : in std_logic;
                output : out std_logic
            );
        end SimpleIO;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.empty());
    REQUIRE(entity->port_clause.ports.size() == 2);
}
