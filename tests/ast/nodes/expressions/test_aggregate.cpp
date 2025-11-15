#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Aggregate: Positional aggregate", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(2 downto 0);
        begin
            vec <= ('1', '0', '1');
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Aggregates are represented as GroupExpr in the generic AST
    const auto *agg = std::get_if<ast::GroupExpr>(&assign->value);
    REQUIRE(agg != nullptr);
    REQUIRE(agg->children.size() == 3);

    // Each element is wrapped in a BinaryExpr with "=>" operator
    // For positional aggregates, the left side is null and right side has the value
    for (const auto &child : agg->children) {
        const auto *assoc = std::get_if<ast::BinaryExpr>(&child);
        REQUIRE(assoc != nullptr);
        REQUIRE(assoc->op == "=>");
    }
}

TEST_CASE("Aggregate: Named aggregate with others", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(7 downto 0);
        begin
            vec <= (others => '0');
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Aggregates with named associations are represented as GroupExpr
    // containing BinaryExpr nodes with "=>" operator
    const auto *agg = std::get_if<ast::GroupExpr>(&assign->value);
    REQUIRE(agg != nullptr);
    REQUIRE_FALSE(agg->children.empty());

    // The first child should be a BinaryExpr representing "others => '0'"
    const auto *assoc = std::get_if<ast::BinaryExpr>(&agg->children[0]);
    REQUIRE(assoc != nullptr);
    REQUIRE(assoc->op == "=>");
}

TEST_CASE("Aggregate: Mixed positional and named", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(3 downto 0);
        begin
            vec <= ('1', 1 => '0', others => '1');
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Mixed aggregates are also GroupExpr
    const auto *agg = std::get_if<ast::GroupExpr>(&assign->value);
    REQUIRE(agg != nullptr);
    REQUIRE(agg->children.size() == 3);
}

TEST_CASE("Aggregate: Nested aggregate", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            type matrix_t is array(0 to 1, 0 to 1) of std_logic;
            signal matrix : matrix_t;
        begin
            matrix <= (('0', '1'), ('1', '0'));
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Outer aggregate
    const auto *outer_agg = std::get_if<ast::GroupExpr>(&assign->value);
    REQUIRE(outer_agg != nullptr);
    REQUIRE(outer_agg->children.size() == 2);

    // Each child of outer aggregate is a BinaryExpr (element association)
    const auto *first_assoc = std::get_if<ast::BinaryExpr>(&outer_agg->children[0]);
    REQUIRE(first_assoc != nullptr);
    REQUIRE(first_assoc->op == "=>");

    // The right side of the association should be the inner aggregate
    REQUIRE(first_assoc->right != nullptr);
    const auto *inner_agg = std::get_if<ast::GroupExpr>(first_assoc->right.get());
    REQUIRE(inner_agg != nullptr);
    REQUIRE(inner_agg->children.size() == 2);
}

TEST_CASE("Aggregate: Range choice in aggregate", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(7 downto 0);
        begin
            vec <= (7 downto 4 => '1', 3 downto 0 => '0');
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Aggregates with range choices
    const auto *agg = std::get_if<ast::GroupExpr>(&assign->value);
    REQUIRE(agg != nullptr);
    REQUIRE(agg->children.size() == 2);

    // Each child should be a named association (BinaryExpr with "=>")
    for (const auto &child : agg->children) {
        const auto *assoc = std::get_if<ast::BinaryExpr>(&child);
        REQUIRE(assoc != nullptr);
        REQUIRE(assoc->op == "=>");
    }
}
