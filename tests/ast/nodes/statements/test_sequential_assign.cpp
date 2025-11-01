#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("SequentialAssign: Simple variable assignment", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                count := 0;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("SequentialAssign: Assignment with literal value", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                result := '1';
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);

    // Check target
    auto *target = std::get_if<ast::TokenExpr>(&assign->target);
    REQUIRE(target != nullptr);
    REQUIRE(target->text == "result");

    // Check value
    auto *value = std::get_if<ast::TokenExpr>(&assign->value);
    REQUIRE(value != nullptr);
    REQUIRE(value->text == "'1'");
}

TEST_CASE("SequentialAssign: Assignment with identifier", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                output := input;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("SequentialAssign: Assignment with arithmetic expression",
          "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                count := count + 1;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);

    // Value should be a binary expression
    auto *value = std::get_if<ast::BinaryExpr>(&assign->value);
    REQUIRE(value != nullptr);
    REQUIRE(value->op == "+");
}

TEST_CASE("SequentialAssign: Assignment with logical expression", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                result := a and b;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);

    auto *value = std::get_if<ast::BinaryExpr>(&assign->value);
    REQUIRE(value != nullptr);
    REQUIRE(value->op == "and");
}

TEST_CASE("SequentialAssign: Assignment with unary expression", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                output := not input;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);

    auto *value = std::get_if<ast::UnaryExpr>(&assign->value);
    REQUIRE(value != nullptr);
    REQUIRE(value->op == "not");
}

TEST_CASE("SequentialAssign: Multiple sequential assignments", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                a := 1;
                b := 2;
                c := 3;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 3);

    // Check all three are sequential assignments
    for (size_t i = 0; i < 3; i++) {
        auto *assign = std::get_if<ast::SequentialAssign>(&proc->body[i]);
        REQUIRE(assign != nullptr);
    }
}

TEST_CASE("SequentialAssign: Assignment inside if statement", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if condition then
                    result := '1';
                else
                    result := '0';
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
    REQUIRE_FALSE(if_stmt->if_branch.body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(if_stmt->if_branch.body.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("SequentialAssign: Assignment inside case statement", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                case sel is
                    when 0 =>
                        result := A;
                    when 1 =>
                        result := B;
                    when others =>
                        result := C;
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
    REQUIRE_FALSE(case_stmt->when_clauses.empty());
    REQUIRE_FALSE(case_stmt->when_clauses[0].body.empty());
}

TEST_CASE("SequentialAssign: Assignment inside loop", "[statements][sequential_assign]")
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
    REQUIRE_FALSE(for_loop->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(for_loop->body.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("SequentialAssign: Complex expression assignment", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                result := (a + b) * (c - d);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);
}

TEST_CASE("SequentialAssign: Vector element assignment", "[statements][sequential_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                data(5) := '1';
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *assign = std::get_if<ast::SequentialAssign>(proc->body.data());
    REQUIRE(assign != nullptr);
}
