#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ConcurrentAssign: Simple concurrent signal assignment", "[concurrent][statement]")
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

TEST_CASE("IfStatement: If/elsif/else conditional", "[if][statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if a = 1 then
                    b := 1;
                elsif a = 2 then
                    b := 2;
                else
                    b := 3;
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *if_stmt = std::get_if<ast::IfStatement>(proc->body.data());
    REQUIRE(if_stmt != nullptr);
    REQUIRE(if_stmt->elsif_branches.size() == 1);
    REQUIRE(if_stmt->else_branch.has_value());
}

TEST_CASE("CaseStatement: Case with when clauses", "[case][statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                case state is
                    when IDLE =>
                        next_state := RUNNING;
                    when RUNNING =>
                        next_state := DONE;
                    when others =>
                        next_state := IDLE;
                end case;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *case_stmt = std::get_if<ast::CaseStatement>(proc->body.data());
    REQUIRE(case_stmt != nullptr);
    REQUIRE(case_stmt->when_clauses.size() == 3);
}

TEST_CASE("ForLoop: For loop statement", "[loop][statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                for i in 0 to 10 loop
                    sum := sum + i;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *for_loop = std::get_if<ast::ForLoop>(proc->body.data());
    REQUIRE(for_loop != nullptr);
    REQUIRE(for_loop->iterator == "i");
}

TEST_CASE("WhileLoop: While loop statement", "[loop][statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while count < 10 loop
                    count := count + 1;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
}
