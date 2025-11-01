#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("SequentialAssign: Simple variable assignment", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                count := 0;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("SequentialAssign: Assignment with literal value", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                result := '1';
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);

    // Check target
    auto *target = std::get_if<ast::TokenExpr>(&assign->target);
    REQUIRE(target != nullptr);
    REQUIRE(target->text == "result");

    // Check value
    auto *value = std::get_if<ast::TokenExpr>(&assign->value);
    REQUIRE(value != nullptr);
    REQUIRE(value->text == "'1'");
}
