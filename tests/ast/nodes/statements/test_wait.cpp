#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("WaitStatement: Wait until condition", "[statements][wait]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait until clk = '1';
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

    auto *wait_stmt = std::get_if<ast::WaitStatement>(&proc->body[0]);
    REQUIRE(wait_stmt != nullptr);
    REQUIRE(wait_stmt->condition.has_value());
    REQUIRE_FALSE(wait_stmt->timeout.has_value());
    REQUIRE(wait_stmt->sensitivity_list.empty());
}

TEST_CASE("WaitStatement: Wait for timeout", "[statements][wait]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait for 10 ns;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *wait_stmt = std::get_if<ast::WaitStatement>(&proc->body[0]);
    REQUIRE(wait_stmt != nullptr);
    REQUIRE(wait_stmt->timeout.has_value());
    REQUIRE_FALSE(wait_stmt->condition.has_value());
    REQUIRE(wait_stmt->sensitivity_list.empty());
}

TEST_CASE("WaitStatement: Wait on sensitivity list", "[statements][wait]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait on clk, rst;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *wait_stmt = std::get_if<ast::WaitStatement>(&proc->body[0]);
    REQUIRE(wait_stmt != nullptr);
    REQUIRE(wait_stmt->sensitivity_list.size() == 2);
    REQUIRE(wait_stmt->sensitivity_list[0] == "clk");
    REQUIRE(wait_stmt->sensitivity_list[1] == "rst");
    REQUIRE_FALSE(wait_stmt->condition.has_value());
    REQUIRE_FALSE(wait_stmt->timeout.has_value());
}

TEST_CASE("WaitStatement: Wait with multiple clauses", "[statements][wait]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait on clk until rising_edge(clk) for 100 ns;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *wait_stmt = std::get_if<ast::WaitStatement>(&proc->body[0]);
    REQUIRE(wait_stmt != nullptr);
    REQUIRE(wait_stmt->sensitivity_list.size() == 1);
    REQUIRE(wait_stmt->condition.has_value());
    REQUIRE(wait_stmt->timeout.has_value());
}

TEST_CASE("WaitStatement: Plain wait", "[statements][wait]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *wait_stmt = std::get_if<ast::WaitStatement>(&proc->body[0]);
    REQUIRE(wait_stmt != nullptr);
    REQUIRE(wait_stmt->sensitivity_list.empty());
    REQUIRE_FALSE(wait_stmt->condition.has_value());
    REQUIRE_FALSE(wait_stmt->timeout.has_value());
}
