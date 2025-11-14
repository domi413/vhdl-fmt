#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("BinaryExpr: Simple binary expression with logical operator",
          "[expressions][binary_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= a and b;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(&assign->value);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "and");
}

TEST_CASE("BinaryExpr: Range expression with downto", "[expressions][binary_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : out std_logic_vector(7 downto 0));
        end E;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    const auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    const auto &port = entity->port_clause.ports[0];
    REQUIRE(port.constraint.has_value());

    const auto *index_constraint = std::get_if<ast::IndexConstraint>(&port.constraint.value());
    REQUIRE(index_constraint != nullptr);
}

TEST_CASE("BinaryExpr: Arithmetic expression with multiple operators", "[expressions][binary_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant RESULT : integer := 10 + 5;
        begin
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    const auto *constant = std::get_if<ast::ConstantDecl>(&arch->decls[0]);
    REQUIRE(constant != nullptr);
    REQUIRE(constant->init_expr.has_value());

    const auto *binary = std::get_if<ast::BinaryExpr>(&constant->init_expr.value());
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "+");
}
