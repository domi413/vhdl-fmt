#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("makeNextStatement: Simple next", "[statements][next]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    next;
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

    auto *next_stmt = std::get_if<ast::NextStatement>(&for_loop->body[0]);
    REQUIRE(next_stmt != nullptr);
    REQUIRE_FALSE(next_stmt->loop_label.has_value());
    REQUIRE_FALSE(next_stmt->condition.has_value());
}

TEST_CASE("makeNextStatement: Next with condition", "[statements][next]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    next when i = 5;
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

    auto *next_stmt = std::get_if<ast::NextStatement>(&for_loop->body[0]);
    REQUIRE(next_stmt != nullptr);
    REQUIRE(next_stmt->condition.has_value());
    REQUIRE_FALSE(next_stmt->loop_label.has_value());
}

TEST_CASE("makeNextStatement: Next with loop label", "[statements][next]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                outer_loop: for i in 0 to 10 loop
                    inner_loop: for j in 0 to 5 loop
                        next outer_loop;
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

    auto *next_stmt = std::get_if<ast::NextStatement>(&inner_loop->body[0]);
    REQUIRE(next_stmt != nullptr);
    REQUIRE(next_stmt->loop_label.has_value());
    REQUIRE(next_stmt->loop_label.value() == "outer_loop");
    REQUIRE_FALSE(next_stmt->condition.has_value());
}

TEST_CASE("makeNextStatement: Next with label and condition", "[statements][next]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                outer_loop: for i in 0 to 10 loop
                    next outer_loop when i > 5;
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

    auto *next_stmt = std::get_if<ast::NextStatement>(&for_loop->body[0]);
    REQUIRE(next_stmt != nullptr);
    REQUIRE(next_stmt->loop_label.has_value());
    REQUIRE(next_stmt->condition.has_value());
}
