#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <string_view>
#include <variant>

TEST_CASE("ConcurrentAssign: Simple signal assignment", "[statements][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            output <= input;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *assign = std::get_if<ast::ConcurrentAssign>(arch->stmts.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("ConcurrentAssign: Assignment with literal", "[statements][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            output <= '1';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *assign = std::get_if<ast::ConcurrentAssign>(arch->stmts.data());
    REQUIRE(assign != nullptr);

    // Check target
    auto *target = std::get_if<ast::TokenExpr>(&assign->target);
    REQUIRE(target != nullptr);
    REQUIRE(target->text == "output");

    // Check value
    auto *value = std::get_if<ast::TokenExpr>(&assign->value);
    REQUIRE(value != nullptr);
    REQUIRE(value->text == "'1'");
}

TEST_CASE("ConcurrentAssign: Multiple assignments", "[statements][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            out1 <= in1;
            out2 <= in2;
            out3 <= in3;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 3);

    // Check all three are concurrent assignments
    for (size_t i = 0; i < 3; i++) {
        auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[i]);
        REQUIRE(assign != nullptr);
    }
}
