#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ReportStatement: Simple report", "[statements][report]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                report "Information message";
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

    auto *report_stmt = std::get_if<ast::ReportStatement>(&proc->body[0]);
    REQUIRE(report_stmt != nullptr);
    REQUIRE_FALSE(report_stmt->severity_expr.has_value());
}

TEST_CASE("ReportStatement: Report with severity", "[statements][report]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                report "Error occurred" severity error;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *report_stmt = std::get_if<ast::ReportStatement>(&proc->body[0]);
    REQUIRE(report_stmt != nullptr);
    REQUIRE(report_stmt->severity_expr.has_value());
}

TEST_CASE("ReportStatement: Report with warning severity", "[statements][report]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                report "Warning message" severity warning;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *report_stmt = std::get_if<ast::ReportStatement>(&proc->body[0]);
    REQUIRE(report_stmt != nullptr);
    REQUIRE(report_stmt->severity_expr.has_value());
}

TEST_CASE("ReportStatement: Report with note severity", "[statements][report]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                report "Informational note" severity note;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *report_stmt = std::get_if<ast::ReportStatement>(&proc->body[0]);
    REQUIRE(report_stmt != nullptr);
    REQUIRE(report_stmt->severity_expr.has_value());
}
