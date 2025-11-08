#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("makeAssertStatement: Simple assert", "[statements][assert]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                assert condition;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    auto *assert_stmt = std::get_if<ast::AssertStatement>(&proc->body[0]);
    REQUIRE(assert_stmt != nullptr);
    REQUIRE_FALSE(assert_stmt->report_expr.has_value());
    REQUIRE_FALSE(assert_stmt->severity_expr.has_value());
}

TEST_CASE("makeAssertStatement: Assert with report", "[statements][assert]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                assert condition report "Error message";
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *assert_stmt = std::get_if<ast::AssertStatement>(&proc->body[0]);
    REQUIRE(assert_stmt != nullptr);
    REQUIRE(assert_stmt->report_expr.has_value());
    REQUIRE_FALSE(assert_stmt->severity_expr.has_value());
}

TEST_CASE("makeAssertStatement: Assert with report and severity", "[statements][assert]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                assert condition report "Error message" severity error;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *assert_stmt = std::get_if<ast::AssertStatement>(&proc->body[0]);
    REQUIRE(assert_stmt != nullptr);
    REQUIRE(assert_stmt->report_expr.has_value());
    REQUIRE(assert_stmt->severity_expr.has_value());
}

TEST_CASE("makeAssertStatement: Assert false condition", "[statements][assert]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                assert false report "Always fails";
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *assert_stmt = std::get_if<ast::AssertStatement>(&proc->body[0]);
    REQUIRE(assert_stmt != nullptr);
    REQUIRE(assert_stmt->report_expr.has_value());
}
