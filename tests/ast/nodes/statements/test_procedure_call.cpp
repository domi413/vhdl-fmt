#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ProcedureCallStatement: Simple procedure call", "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                my_procedure;
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

    auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->procedure_name == "my_procedure");
    REQUIRE_FALSE(call_stmt->args.has_value());
}

TEST_CASE("ProcedureCallStatement: Procedure call with single argument",
          "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                write(output_line);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->procedure_name == "write");
    REQUIRE(call_stmt->args.has_value());
}

TEST_CASE("ProcedureCallStatement: Procedure call with multiple arguments",
          "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                assert_equal(expected, actual, "Values should match");
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->procedure_name == "assert_equal");
    REQUIRE(call_stmt->args.has_value());
}

TEST_CASE("ProcedureCallStatement: Procedure call with named associations",
          "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                configure(enable => '1', mode => 2);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->procedure_name == "configure");
    REQUIRE(call_stmt->args.has_value());
}

TEST_CASE("ProcedureCallStatement: Procedure call with qualified name",
          "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                std.textio.write(output_line, message);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->procedure_name == "std.textio.write");
    REQUIRE(call_stmt->args.has_value());
}

TEST_CASE("ProcedureCallStatement: Builtin procedure call", "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait_for(10 ns);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::ProcessStatement>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);

    auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->procedure_name == "wait_for");
}
