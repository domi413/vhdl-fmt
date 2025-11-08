#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("makeExitStatement: Simple exit", "[statements][exit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    exit;
                end loop;
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

    auto *for_loop = std::get_if<ast::ForLoopStatement>(&proc->body[0]);
    REQUIRE(for_loop != nullptr);
    REQUIRE(for_loop->body.size() == 1);

    auto *exit_stmt = std::get_if<ast::ExitStatement>(&for_loop->body[0]);
    REQUIRE(exit_stmt != nullptr);
    REQUIRE_FALSE(exit_stmt->loop_label.has_value());
    REQUIRE_FALSE(exit_stmt->condition.has_value());
}

TEST_CASE("makeExitStatement: Exit with condition", "[statements][exit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    exit when i = 5;
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

    auto *exit_stmt = std::get_if<ast::ExitStatement>(&for_loop->body[0]);
    REQUIRE(exit_stmt != nullptr);
    REQUIRE(exit_stmt->condition.has_value());
    REQUIRE_FALSE(exit_stmt->loop_label.has_value());
}

TEST_CASE("makeExitStatement: Exit with loop label", "[statements][exit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                outer_loop: for i in 0 to 10 loop
                    inner_loop: for j in 0 to 5 loop
                        exit outer_loop;
                    end loop;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *outer_loop = std::get_if<ast::ForLoopStatement>(&proc->body[0]);
    REQUIRE(outer_loop != nullptr);

    auto *inner_loop = std::get_if<ast::ForLoopStatement>(&outer_loop->body[0]);
    REQUIRE(inner_loop != nullptr);

    auto *exit_stmt = std::get_if<ast::ExitStatement>(&inner_loop->body[0]);
    REQUIRE(exit_stmt != nullptr);
    REQUIRE(exit_stmt->loop_label.has_value());
    REQUIRE(exit_stmt->loop_label.value() == "outer_loop");
    REQUIRE_FALSE(exit_stmt->condition.has_value());
}

TEST_CASE("makeExitStatement: Exit with label and condition", "[statements][exit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                outer_loop: for i in 0 to 10 loop
                    exit outer_loop when i > 5;
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

    auto *exit_stmt = std::get_if<ast::ExitStatement>(&for_loop->body[0]);
    REQUIRE(exit_stmt != nullptr);
    REQUIRE(exit_stmt->loop_label.has_value());
    REQUIRE(exit_stmt->condition.has_value());
}

TEST_CASE("makeExitStatement: Exit from while loop", "[statements][exit]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while true loop
                    exit when done;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *while_loop = std::get_if<ast::WhileLoopStatement>(&proc->body[0]);
    REQUIRE(while_loop != nullptr);

    auto *exit_stmt = std::get_if<ast::ExitStatement>(&while_loop->body[0]);
    REQUIRE(exit_stmt != nullptr);
    REQUIRE(exit_stmt->condition.has_value());
}
