#include "ast/nodes/declarations.hpp"
#include "ast/nodes/expressions.hpp"
#include "emit/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <utility>

TEST_CASE("GenericParam with single name", "[pretty_printer][declarations]")
{
    const ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer", .is_last = true };

    const auto result = emit::test::render(param);
    REQUIRE(result == "WIDTH : integer");
}

TEST_CASE("GenericParam with multiple names", "[pretty_printer][declarations]")
{
    const ast::GenericParam param{
        .names = { "WIDTH", "HEIGHT", "DEPTH" },
          .type_name = "positive", .is_last = true
    };

    const auto result = emit::test::render(param);
    REQUIRE(result == "WIDTH, HEIGHT, DEPTH : positive");
}

TEST_CASE("GenericParam with default value", "[pretty_printer][declarations]")
{
    const ast::GenericParam param{ .names = { "WIDTH" },
                                   .type_name = "integer",
                                   .default_expr = ast::TokenExpr{ .text = "8" },
                                   .is_last = true };

    const auto result = emit::test::render(param);
    REQUIRE(result == "WIDTH : integer := 8");
}

TEST_CASE("GenericParam with multiple names and default", "[pretty_printer][declarations]")
{
    const ast::GenericParam param{
        .names = { "A", "B" },
        .type_name = "natural",
        .default_expr = ast::TokenExpr{ .text = "0" },
        .is_last = true
    };

    const auto result = emit::test::render(param);
    REQUIRE(result == "A, B : natural := 0");
}

TEST_CASE("Port with single name and mode", "[pretty_printer][declarations]")
{
    const ast::Port port{
        .names = { "clk" }, .mode = "in", .type_name = "std_logic", .is_last = true
    };

    const auto result = emit::test::render(port);
    REQUIRE(result == "clk : in std_logic");
}

TEST_CASE("Port with multiple names", "[pretty_printer][declarations]")
{
    const ast::Port port{
        .names = { "data_in", "data_out" },
        .mode = "inout",
        .type_name = "std_logic_vector",
        .is_last = true
    };

    const auto result = emit::test::render(port);
    REQUIRE(result == "data_in, data_out : inout std_logic_vector");
}

TEST_CASE("Port with constraints", "[pretty_printer][declarations]")
{
    // Create constraint: 7 downto 0
    ast::BinaryExpr constraint{ .left = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "7" }),
                                .op = "downto",
                                .right
                                = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "0" }) };

    // Create IndexConstraint with GroupExpr containing the range
    ast::IndexConstraint idx_constraint;
    idx_constraint.ranges.children.emplace_back(std::move(constraint));

    const ast::Port port{ .names = { "data" },
                          .mode = "in",
                          .type_name = "std_logic_vector",
                          .default_expr = std::nullopt,
                          .constraint = ast::Constraint(std::move(idx_constraint)),
                          .is_last = true };

    const auto result = emit::test::render(port);
    REQUIRE(result == "data : in std_logic_vector(7 downto 0)");
}

TEST_CASE("Port with multiple constraints", "[pretty_printer][declarations]")
{
    // First constraint: 7 downto 0
    ast::BinaryExpr constraint1{ .left = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "7" }),
                                 .op = "downto",
                                 .right
                                 = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "0" }) };

    // Second constraint: 3 downto 0
    ast::BinaryExpr constraint2{ .left = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "3" }),
                                 .op = "downto",
                                 .right
                                 = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "0" }) };

    // Create IndexConstraint with GroupExpr containing both ranges
    ast::IndexConstraint idx_constraint;
    idx_constraint.ranges.children.emplace_back(std::move(constraint1));
    idx_constraint.ranges.children.emplace_back(std::move(constraint2));

    const ast::Port port{ .names = { "matrix" },
                          .mode = "out",
                          .type_name = "matrix_type",
                          .default_expr = std::nullopt,
                          .constraint = ast::Constraint(std::move(idx_constraint)),
                          .is_last = true };

    const auto result = emit::test::render(port);
    REQUIRE(result == "matrix : out matrix_type(7 downto 0, 3 downto 0)");
}

TEST_CASE("Port with default value", "[pretty_printer][declarations]")
{
    const ast::Port port{ .names = { "enable" },
                          .mode = "in",
                          .type_name = "std_logic",
                          .default_expr = ast::TokenExpr{ .text = "'0'" },
                          .is_last = true };

    const auto result = emit::test::render(port);
    REQUIRE(result == "enable : in std_logic := '0'");
}

TEST_CASE("Port with constraints and default value", "[pretty_printer][declarations]")
{
    // Constraint
    ast::BinaryExpr constraint{ .left = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "7" }),
                                .op = "downto",
                                .right
                                = std::make_unique<ast::Expr>(ast::TokenExpr{ .text = "0" }) };

    // Create IndexConstraint with GroupExpr containing the range
    ast::IndexConstraint idx_constraint;
    idx_constraint.ranges.children.emplace_back(std::move(constraint));

    const ast::Port port{ .names = { "data" },
                          .mode = "in",
                          .type_name = "std_logic_vector",
                          .default_expr = ast::TokenExpr{ .text = "X\"00\"" },
                          .constraint = ast::Constraint(std::move(idx_constraint)),
                          .is_last = true };

    const auto result = emit::test::render(port);
    REQUIRE(result == "data : in std_logic_vector(7 downto 0) := X\"00\"");
}
