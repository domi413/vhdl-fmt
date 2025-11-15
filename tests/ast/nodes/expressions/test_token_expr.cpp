#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("TokenExpr: Simple identifier token", "[expressions][token_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a : in std_logic; b : out std_logic);
        end E;
        architecture A of E is
        begin
            b <= a;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    const auto *token_target = std::get_if<ast::TokenExpr>(&assign->target);
    REQUIRE(token_target != nullptr);
    REQUIRE(token_target->text == "b");

    const auto *token_value = std::get_if<ast::TokenExpr>(&assign->value);
    REQUIRE(token_value != nullptr);
    REQUIRE(token_value->text == "a");
}

TEST_CASE("TokenExpr: Numeric literal token", "[expressions][token_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant WIDTH : integer := 8;
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

    const auto *token = std::get_if<ast::TokenExpr>(&constant->init_expr.value());
    REQUIRE(token != nullptr);
    REQUIRE(token->text == "8");
}

TEST_CASE("TokenExpr: Character literal token", "[expressions][token_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a : in std_logic; b : out std_logic);
        end E;
        architecture A of E is
        begin
            process(a)
            begin
                if a = '1' then
                    b <= '0';
                end if;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *proc = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    const auto *if_stmt = std::get_if<ast::IfStatement>(&proc->body[0]);
    REQUIRE(if_stmt != nullptr);
    REQUIRE(if_stmt->if_branch.body.size() == 1);
}
