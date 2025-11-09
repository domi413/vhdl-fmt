#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "emit/test_utils.hpp"
#include "nodes/declarations.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>

TEST_CASE("Empty DesignFile", "[pretty_printer][design_file]")
{
    const ast::DesignFile file{};

    const auto result = emit::test::render(file);
    REQUIRE(result.empty());
}

TEST_CASE("DesignFile with single entity", "[pretty_printer][design_file]")
{
    ast::DesignFile file{};

    ast::Entity entity{ .name = "test_entity" };

    file.units.emplace_back(std::move(entity));

    const std::string result = emit::test::render(file);
    const std::string expected = "entity test_entity is\n"
                                 "end entity test_entity;\n";

    REQUIRE(result == expected);
}

TEST_CASE("DesignFile with entity and architecture", "[pretty_printer][design_file]")
{
    ast::DesignFile file{};

    ast::Entity entity{ .name = "counter" };

    ast::Port port{ .names = { "clk" }, .mode = "in", .type_name = "std_logic" };
    entity.port_clause.ports.push_back(std::move(port));

    // Create architecture
    ast::Architecture arch{ .name = "rtl", .entity_name = "counter" };

    file.units.emplace_back(std::move(entity));
    file.units.emplace_back(std::move(arch));

    const std::string result = emit::test::render(file);
    const std::string expected = "entity counter is\n"
                                 "  port ( clk : in std_logic );\n"
                                 "end entity counter;\n\n"
                                 "architecture rtl of counter is\n"
                                 "begin\n"
                                 "end architecture rtl;\n";

    REQUIRE(result == expected);
}
TEST_CASE("DesignFile with multiple design units", "[pretty_printer][design_file]")
{
    ast::DesignFile file{};

    // First entity
    ast::Entity entity1{ .name = "entity1" };

    // Second entity
    ast::Entity entity2{ .name = "entity2" };

    // Architecture for entity1
    ast::Architecture arch1{ .name = "behavioral", .entity_name = "entity1" };

    file.units.emplace_back(std::move(entity1));
    file.units.emplace_back(std::move(entity2));
    file.units.emplace_back(std::move(arch1));

    const std::string result = emit::test::render(file);
    const std::string expected = "entity entity1 is\n"
                                 "end entity entity1;\n\n"
                                 "entity entity2 is\n"
                                 "end entity entity2;\n\n"
                                 "architecture behavioral of entity1 is\n"
                                 "begin\n"
                                 "end architecture behavioral;\n";

    REQUIRE(result == expected);
}
