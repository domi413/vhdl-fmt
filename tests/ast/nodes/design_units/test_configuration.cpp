#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Configuration: Basic configuration declaration", "[design_units][configuration]")
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
    // TODO(someone):
    // REQUIRE(design.units.size() == 1);
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
    // REQUIRE(config->name == "MyConfig");
    // REQUIRE(config->entity_name == "MyEntity");
}

TEST_CASE("Configuration: With component bindings", "[design_units][configuration]")
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
    // TODO(someone):
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
}

TEST_CASE("Configuration: With generic map", "[design_units][configuration]")
{
    constexpr std::string_view VHDL_FILE = R"(
        configuration MappedConfig of MappedEntity is
            for RTL
                for U1 : Counter
                    use entity work.Counter(RTL)
                        generic map (WIDTH => 16);
                end for;
            end for;
        end MappedConfig;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone):
    // auto *config = std::get_if<ast::Configuration>(&design.units[0]);
    // REQUIRE(config != nullptr);
}
