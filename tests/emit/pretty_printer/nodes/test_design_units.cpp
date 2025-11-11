#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "common/config.hpp"
#include "emit/test_utils.hpp"
#include "nodes/declarations.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

using emit::test::defaultConfig;

TEST_CASE("Simple Entity without generics or ports", "[pretty_printer][design_units]")
{
    const ast::Entity entity{ .name = "simple_entity" };

    const std::string result = emit::test::render(entity);
    constexpr std::string_view EXPECTED = "entity simple_entity is\n"
                                          "end entity simple_entity;";

    REQUIRE(result == EXPECTED);
}

TEST_CASE("Entity with generics", "[pretty_printer][design_units]")
{
    ast::Entity entity{ .name = "configurable" };

    ast::GenericParam param{ .names = { "WIDTH" },
                             .type_name = "positive",
                             .default_expr = ast::TokenExpr{ .text = "8" } };

    entity.generic_clause.generics.emplace_back(std::move(param));

    const std::string result = emit::test::render(entity);
    constexpr std::string_view EXPECTED = "entity configurable is\n"
                                          "  generic ( WIDTH : positive := 8 );\n"
                                          "end entity configurable;";

    REQUIRE(result == EXPECTED);
}

TEST_CASE("Entity with ports", "[pretty_printer][design_units]")
{
    ast::Entity entity{ .name = "counter" };

    // Add ports
    ast::Port port1{ .names = { "clk" }, .mode = "in", .type_name = "std_logic" };

    ast::Port port2{ .names = { "count" }, .mode = "out", .type_name = "natural" };

    entity.port_clause.ports.emplace_back(std::move(port1));
    entity.port_clause.ports.emplace_back(std::move(port2));

    const std::string result = emit::test::render(entity);
    constexpr std::string_view EXPECTED = "entity counter is\n"
                                          "  port ( clk : in std_logic; count : out natural );\n"
                                          "end entity counter;";
    REQUIRE(result == EXPECTED);
}

TEST_CASE("Entity with generics and ports", "[pretty_printer][design_units]")
{
    ast::Entity entity{ .name = "fifo" };

    // Add generic
    ast::GenericParam param{ .names = { "DEPTH" },
                             .type_name = "positive",
                             .default_expr = ast::TokenExpr{ .text = "16" } };
    entity.generic_clause.generics.emplace_back(std::move(param));

    // Add constraint
    auto left = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "7" });
    auto right = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "0" });
    ast::BinaryExpr constraint{ .left = std::move(left),
                                .op = "downto",
                                .right = std::move(right) };

    // Create IndexConstraint with GroupExpr containing the range
    ast::IndexConstraint idx_constraint;
    idx_constraint.ranges.children.emplace_back(std::move(constraint));

    // Add port
    ast::Port port{ .names = { "data_in" },
                    .mode = "in",
                    .type_name = "std_logic_vector",
                    .default_expr = std::nullopt,
                    .constraint = ast::Constraint(std::move(idx_constraint)) };

    entity.port_clause.ports.emplace_back(std::move(port));

    const std::string result = emit::test::render(entity);
    constexpr std::string_view EXPECTED = "entity fifo is\n"
                                          "  generic ( DEPTH : positive := 16 );\n"
                                          "  port ( data_in : in std_logic_vector(7 downto 0) );\n"
                                          "end entity fifo;";

    REQUIRE(result == EXPECTED);
}

TEST_CASE("Entity with custom end label", "[pretty_printer][design_units]")
{
    const ast::Entity entity{ .name = "my_entity", .end_label = "custom_label" };

    const std::string result = emit::test::render(entity);
    constexpr std::string_view EXPECTED = "entity my_entity is\n"
                                          "end entity custom_label;";

    REQUIRE(result == EXPECTED);
}

TEST_CASE("Entity with custom indent size (4 spaces)", "[pretty_printer][design_units][config]")
{
    ast::Entity entity{ .name = "configurable" };

    ast::GenericParam param{ .names = { "WIDTH" },
                             .type_name = "positive",
                             .default_expr = ast::TokenExpr{ .text = "8" } };

    entity.generic_clause.generics.emplace_back(std::move(param));

    // Create config with 4-space indent
    auto config = defaultConfig();
    config.line_config.indent_size = 4;

    const std::string result = emit::test::render(entity, config);
    constexpr std::string_view EXPECTED = "entity configurable is\n"
                                          "    generic ( WIDTH : positive := 8 );\n"
                                          "end entity configurable;";

    REQUIRE(result == EXPECTED);
}

TEST_CASE("Simple Architecture", "[pretty_printer][design_units]")
{
    const ast::Architecture arch{ .name = "rtl", .entity_name = "counter" };

    const std::string result = emit::test::render(arch);
    constexpr std::string_view EXPECTED = "architecture rtl of counter is\n"
                                          "begin\n"
                                          "end architecture rtl;";

    REQUIRE(result == EXPECTED);
}
