#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Configuration Declaration
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Configuration declaration for binding components",
          "[configuration][design_unit][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration MyConfig of MyEntity is
            for RTL
                for U1 : MyComponent use entity work.MyComponent(RTL);
                end for;
            end for;
        end MyConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "MyConfig");
    // REQUIRE(config->entity_name == "MyEntity");
    // REQUIRE(config->end_label.has_value());
    // REQUIRE(config->end_label.value() == "MyConfig");
}

// -----------------------------------------------------------------------------
// Configuration with Component Bindings
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Captures component bindings", "[configuration][binding][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration TestConfig of TestEntity is
            for Behavioral
                for U1 : Adder use entity work.Adder(RTL);
                end for;
                for U2 : Multiplier use entity work.Multiplier(Fast);
                end for;
            end for;
        end TestConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "TestConfig");
    // REQUIRE(config->entity_name == "TestEntity");
}

// -----------------------------------------------------------------------------
// Configuration with Nested For Blocks
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Handles nested for blocks", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration NestedConfig of TopLevel is
            for Structural
                for U1 : SubModule
                    for RTL
                        for U2 : BasicUnit use entity work.BasicUnit(Optimized);
                        end for;
                    end for;
                end for;
            end for;
        end NestedConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "NestedConfig");
}

// -----------------------------------------------------------------------------
// Configuration without End Label
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Can omit end label", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration SimpleConfig of SimpleEntity is
            for RTL
            end for;
        end;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "SimpleConfig");
}

// -----------------------------------------------------------------------------
// Configuration with All/Others
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Supports all/others specifications", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration AllConfig of TestEntity is
            for RTL
                for all : Adder use entity work.Adder(RTL);
                end for;
                for others : Component use entity work.Component(Default);
                end for;
            end for;
        end AllConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "AllConfig");
}

// -----------------------------------------------------------------------------
// Configuration with Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Handles generic map in bindings", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration MappedConfig of MappedEntity is
            for RTL
                for U1 : Counter
                    use entity work.Counter(RTL)
                        generic map (WIDTH => 16, RESET_VAL => 0);
                end for;
            end for;
        end MappedConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "MappedConfig");
}

// -----------------------------------------------------------------------------
// Configuration with Port Map
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Handles port map in bindings", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration PortConfig of PortEntity is
            for RTL
                for U1 : Module
                    use entity work.Module(Behavioral)
                        port map (clk => system_clk, rst => reset_n);
                end for;
            end for;
        end PortConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "PortConfig");
}

// -----------------------------------------------------------------------------
// Configuration with Configuration Specifications
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Supports configuration specifications", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration SpecConfig of SpecEntity is
            for RTL
                for U1 : SubEntity
                    use configuration work.SubEntityConfig;
                end for;
            end for;
        end SpecConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "SpecConfig");
}

// -----------------------------------------------------------------------------
// Minimal Configuration
// -----------------------------------------------------------------------------
TEST_CASE("Configuration: Minimal configuration with empty for block", "[configuration][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration MinConfig of MinEntity is
            for RTL
            end for;
        end MinConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Configuration design unit is implemented
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "MinConfig");
    // REQUIRE(config->entity_name == "MinEntity");
}
