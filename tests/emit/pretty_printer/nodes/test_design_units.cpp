#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "common/config.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/test_utils.hpp"
#include "nodes/declarations.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <utility>

using emit::test::defaultConfig;

TEST_CASE("Simple Entity without generics or ports", "[pretty_printer][design_units]")
{
    ast::Entity entity;
    entity.name = "simple_entity";

    emit::PrettyPrinter printer;
    const auto doc = printer(entity);
    const auto result = doc.render(defaultConfig());

    const std::string expected = "entity simple_entity is\n"
                                 "end entity simple_entity;";

    REQUIRE(result == expected);
}

TEST_CASE("Entity with generics", "[pretty_printer][design_units]")
{
    ast::Entity entity;
    entity.name = "configurable";

    // Add generic
    ast::GenericParam param;
    param.names = { "WIDTH" };
    param.type_name = "positive";
    ast::TokenExpr default_val;
    default_val.text = "8";
    param.default_expr = default_val;
    entity.generic_clause.generics.push_back(std::move(param));

    emit::PrettyPrinter printer;
    const auto doc = printer(entity);
    const auto result = doc.render(defaultConfig());

    const std::string expected = "entity configurable is\n"
                                 "  generic ( WIDTH : positive := 8 );\n"
                                 "end entity configurable;";

    REQUIRE(result == expected);
}

TEST_CASE("Entity with ports", "[pretty_printer][design_units]")
{
    ast::Entity entity;
    entity.name = "counter";

    // Add ports
    ast::Port port1;
    port1.names = { "clk" };
    port1.mode = "in";
    port1.type_name = "std_logic";

    ast::Port port2;
    port2.names = { "count" };
    port2.mode = "out";
    port2.type_name = "natural";

    entity.port_clause.ports.push_back(std::move(port1));
    entity.port_clause.ports.push_back(std::move(port2));

    emit::PrettyPrinter printer;
    const auto doc = printer(entity);
    const auto result = doc.render(defaultConfig());

    const std::string expected = "entity counter is\n"
                                 "  port ( clk : in std_logic; count : out natural );\n"
                                 "end entity counter;";
    REQUIRE(result == expected);
}

TEST_CASE("Entity with generics and ports", "[pretty_printer][design_units]")
{
    ast::Entity entity;
    entity.name = "fifo";

    // Add generic
    ast::GenericParam param;
    param.names = { "DEPTH" };
    param.type_name = "positive";
    ast::TokenExpr default_val;
    default_val.text = "16";
    param.default_expr = default_val;
    entity.generic_clause.generics.push_back(std::move(param));

    // Add port
    ast::Port port;
    port.names = { "data_in" };
    port.mode = "in";
    port.type_name = "std_logic_vector";

    // Add constraint
    ast::BinaryExpr constraint;
    ast::TokenExpr left_token;
    left_token.text = "7";
    ast::TokenExpr right_token;
    right_token.text = "0";
    auto left = std::make_unique<ast::Expr>(left_token);
    auto right = std::make_unique<ast::Expr>(right_token);
    constraint.left = std::move(left);
    constraint.op = "downto";
    constraint.right = std::move(right);
    port.constraints.push_back(std::move(constraint));

    entity.port_clause.ports.push_back(std::move(port));

    emit::PrettyPrinter printer;
    const auto doc = printer(entity);
    const auto result = doc.render(defaultConfig());

    // Both clauses fit on one line
    const std::string expected = "entity fifo is\n"
                                 "  generic ( DEPTH : positive := 16 );\n"
                                 "  port ( data_in : in std_logic_vector(7 downto 0) );\n"
                                 "end entity fifo;";

    REQUIRE(result == expected);
}

TEST_CASE("Entity with custom end label", "[pretty_printer][design_units]")
{
    ast::Entity entity;
    entity.name = "my_entity";
    entity.end_label = "custom_label";

    emit::PrettyPrinter printer;
    const auto doc = printer(entity);
    const auto result = doc.render(defaultConfig());

    const std::string expected = "entity my_entity is\n"
                                 "end entity custom_label;";

    REQUIRE(result == expected);
}

TEST_CASE("Entity with custom indent size (4 spaces)", "[pretty_printer][design_units][config]")
{
    ast::Entity entity;
    entity.name = "configurable";

    // Add generic
    ast::GenericParam param;
    param.names = { "WIDTH" };
    param.type_name = "positive";
    ast::TokenExpr default_val;
    default_val.text = "8";
    param.default_expr = default_val;
    entity.generic_clause.generics.push_back(std::move(param));

    // Create config with 4-space indent
    auto config = defaultConfig();
    config.line_config.indent_size = 4;

    emit::PrettyPrinter printer(config);

    const auto doc = printer(entity);
    const auto result = doc.render(config);

    // Indentation should be 4 spaces now
    const std::string expected = "entity configurable is\n"
                                 "    generic ( WIDTH : positive := 8 );\n"
                                 "end entity configurable;";

    REQUIRE(result == expected);
}

TEST_CASE("Simple Architecture", "[pretty_printer][design_units]")
{
    ast::Architecture arch;
    arch.name = "rtl";
    arch.entity_name = "counter";

    emit::PrettyPrinter printer;
    const auto doc = printer(arch);
    const auto result = doc.render(defaultConfig());

    const std::string expected = "architecture rtl of counter is\n"
                                 "begin\n"
                                 "end architecture rtl;";

    REQUIRE(result == expected);
}
