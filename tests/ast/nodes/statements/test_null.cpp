#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("NullStatement: Simple null statement", "[statements][null]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                null;
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

    auto *null_stmt = std::get_if<ast::NullStatement>(&proc->body[0]);
    REQUIRE(null_stmt != nullptr);
    REQUIRE_FALSE(null_stmt->label.has_value());
}

TEST_CASE("NullStatement: Null in if branch", "[statements][null]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if condition then
                    null;
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *if_stmt = std::get_if<ast::IfStatement>(&proc->body[0]);
    REQUIRE(if_stmt != nullptr);
    REQUIRE(if_stmt->if_branch.body.size() == 1);

    auto *null_stmt = std::get_if<ast::NullStatement>(&if_stmt->if_branch.body[0]);
    REQUIRE(null_stmt != nullptr);
}

TEST_CASE("NullStatement: Null in case when", "[statements][null]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                case state is
                    when idle =>
                        null;
                    when others =>
                        null;
                end case;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *case_stmt = std::get_if<ast::CaseStatement>(&proc->body[0]);
    REQUIRE(case_stmt != nullptr);
    REQUIRE(case_stmt->when_clauses.size() >= 1);

    auto *null_stmt = std::get_if<ast::NullStatement>(&case_stmt->when_clauses[0].body[0]);
    REQUIRE(null_stmt != nullptr);
}

TEST_CASE("NullStatement: Null in loop", "[statements][null]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    null;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *for_loop = std::get_if<ast::ForLoopStatement>(&proc->body[0]);
    REQUIRE(for_loop != nullptr);

    auto *null_stmt = std::get_if<ast::NullStatement>(&for_loop->body[0]);
    REQUIRE(null_stmt != nullptr);
}

TEST_CASE("NullStatement: Multiple null statements", "[statements][null]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                null;
                null;
                null;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 3);

    for (const auto &stmt : proc->body) {
        auto *null_stmt = std::get_if<ast::NullStatement>(&stmt);
        REQUIRE(null_stmt != nullptr);
    }
}
