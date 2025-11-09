#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "emit/test_utils.hpp"
#include "nodes/declarations.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <string>
#include <utility>

TEST_CASE("Empty GenericClause", "[pretty_printer][clauses]")
{
    const ast::GenericClause clause{};

    const auto result = emit::test::render(clause);
    REQUIRE(result.empty());
}

TEST_CASE("GenericClause with single parameter (flat)", "[pretty_printer][clauses]")
{
    ast::GenericClause clause{};

    ast::GenericParam param{ .names = { "WIDTH" },
                             .type_name = "integer",
                             .default_expr = ast::TokenExpr{ .text = "8" } };

    clause.generics.push_back(std::move(param));

    const auto result = emit::test::render(clause);
    REQUIRE(result == "generic ( WIDTH : integer := 8 );");
}

TEST_CASE("GenericClause with multiple parameters (flat)", "[pretty_printer][clauses]")
{
    ast::GenericClause clause{};

    ast::GenericParam param1{ .names = { "WIDTH" },
                              .type_name = "positive",
                              .default_expr = ast::TokenExpr{ .text = "8" } };

    ast::GenericParam param2{ .names = { "HEIGHT" },
                              .type_name = "positive",
                              .default_expr = ast::TokenExpr{ .text = "16" } };

    clause.generics.push_back(std::move(param1));
    clause.generics.push_back(std::move(param2));

    const auto result = emit::test::render(clause);
    REQUIRE(result == "generic ( WIDTH : positive := 8; HEIGHT : positive := 16 );");
}

TEST_CASE("Empty PortClause", "[pretty_printer][clauses]")
{
    const ast::PortClause clause{};

    const auto result = emit::test::render(clause);
    REQUIRE(result.empty());
}

TEST_CASE("PortClause with single port (flat)", "[pretty_printer][clauses]")
{
    ast::PortClause clause{};

    ast::Port port{ .names = { "clk" }, .mode = "in", .type_name = "std_logic" };

    clause.ports.push_back(std::move(port));

    const auto result = emit::test::render(clause);
    REQUIRE(result == "port ( clk : in std_logic );");
}
TEST_CASE("PortClause with multiple ports (broken)", "[pretty_printer][clauses]")
{
    ast::PortClause clause{};

    ast::Port port1{ .names = { "clk" }, .mode = "in", .type_name = "std_logic" };
    ast::Port port2{ .names = { "reset" }, .mode = "in", .type_name = "std_logic" };

    // Build constraint for port3
    auto left = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "7" });
    auto right = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "0" });
    ast::BinaryExpr constraint{ .left = std::move(left),
                                .op = "downto",
                                .right = std::move(right) };

    ast::IndexConstraint idx_constraint{};
    idx_constraint.ranges.children.emplace_back(std::move(constraint));

    ast::Port port3{ .names = { "data_out" },
                     .mode = "out",
                     .type_name = "std_logic_vector",
                     .default_expr = std::nullopt,
                     .constraint = ast::Constraint{ std::move(idx_constraint) } };

    clause.ports.emplace_back(std::move(port1));
    clause.ports.emplace_back(std::move(port2));
    clause.ports.emplace_back(std::move(port3));

    const std::string result = emit::test::render(clause);
    const std::string expected = "port (\n"
                                 "  clk : in std_logic;\n"
                                 "  reset : in std_logic;\n"
                                 "  data_out : out std_logic_vector(7 downto 0)\n"
                                 ");";

    REQUIRE(result == expected);
}

TEST_CASE("PortClause with align_signals disabled", "[pretty_printer][clauses][config]")
{
    ast::PortClause clause{};

    ast::Port port1{ .names = { "clk" }, .mode = "in", .type_name = "std_logic" };
    ast::Port port2{ .names = { "data_valid" }, .mode = "in", .type_name = "std_logic" };
    ast::Port port3{ .names = { "output_signal" }, .mode = "out", .type_name = "std_logic_vector" };

    clause.ports.emplace_back(std::move(port1));
    clause.ports.emplace_back(std::move(port2));
    clause.ports.emplace_back(std::move(port3));

    // Should use broken layout without alignment
    const auto result = emit::test::render(clause);
    const std::string expected = "port (\n"
                                 "  clk : in std_logic;\n"
                                 "  data_valid : in std_logic;\n"
                                 "  output_signal : out std_logic_vector\n"
                                 ");";
    REQUIRE(result == expected);
}

// TODO(vedivad): Enable when table combinator is implemented
// TEST_CASE("PortClause with align_signals enabled", "[pretty_printer][clauses][config]")
// {
//     ast::PortClause clause;

//     ast::Port port1;
//     port1.names = { "clk" };
//     port1.mode = "in";
//     port1.type_name = "std_logic";

//     ast::Port port2;
//     port2.names = { "data_valid" };
//     port2.mode = "in";
//     port2.type_name = "std_logic";

//     ast::Port port3;
//     port3.names = { "output_signal" };
//     port3.mode = "out";
//     port3.type_name = "std_logic_vector";

//     clause.ports.emplace_back(std::move(port1));
//     clause.ports.emplace_back(std::move(port2));
//     clause.ports.emplace_back(std::move(port3));

//     // Create config with align_signals enabled
//     constexpr int TEST_LINE_LENGTH = 80;
//     common::Config config;
//     config.port_map.align_signals = true;
//     config.line_config.line_length = TEST_LINE_LENGTH;
//     config.line_config.indent_size = 2;

//     emit::PrettyPrinter printer(config);
//     const auto doc = printer(clause);
//     const auto result = doc.render(config);

//     // Should use table format with aligned columns
//     const std::string expected = "port (\n"
//                                  "  clk           : in  std_logic;\n"
//                                  "  data_valid    : in  std_logic;\n"
//                                  "  output_signal : out std_logic_vector\n"
//                                  ");";
//     REQUIRE(result == expected);
// }
