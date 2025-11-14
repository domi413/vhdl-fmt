#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ParenExpr: Parenthesized expression for precedence", "[expressions][paren_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b, c : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= (a and b) or c;
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
    REQUIRE(binary->op == "or");
}

TEST_CASE("ParenExpr: Nested parentheses", "[expressions][paren_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant RESULT : integer := ((5 + 3) * 2);
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

    const auto *paren = std::get_if<ast::ParenExpr>(&constant->init_expr.value());
    REQUIRE(paren != nullptr);
}
