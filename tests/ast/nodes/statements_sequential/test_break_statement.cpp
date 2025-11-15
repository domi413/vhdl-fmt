#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
TEST_CASE("BreakStatement: Simple break statement", "[statements_sequential][break_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
            begin
                break when true;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto& arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto& proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);

    const auto& break_stmt = std::get<ast::BreakStatement>(proc.body[0]);
    REQUIRE(break_stmt.break_elements.empty());
    // Simple break with when condition
    REQUIRE(break_stmt.condition.has_value());
}

TEST_CASE("BreakStatement: Break with elements", "[statements_sequential][break_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            quantity q1, q2 : real;
        begin
            process
            begin
                break q1 => 1.0, q2 => 2.0 when clk = '1';
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto& arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto& proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);

    const auto& break_stmt = std::get<ast::BreakStatement>(proc.body[0]);
    // Break with elements and when condition
    REQUIRE_FALSE(break_stmt.break_elements.empty());
    REQUIRE(break_stmt.condition.has_value());
}
