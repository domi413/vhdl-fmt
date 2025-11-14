#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("WhileLoop: Simple while loop", "[statements_sequential][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
        architecture A of E is
        begin
            process(clk)
                variable counter : integer := 0;
            begin
                while counter < 10 loop
                end loop;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *proc = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    const auto *while_loop = std::get_if<ast::WhileLoop>(&proc->body[0]);
    REQUIRE(while_loop != nullptr);
    REQUIRE(while_loop->body.empty());
}

TEST_CASE("WhileLoop: While loop with boolean condition", "[statements_sequential][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
        architecture A of E is
        begin
            process(clk)
                variable done : boolean := false;
            begin
                while not done loop
                end loop;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *proc = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    const auto *while_loop = std::get_if<ast::WhileLoop>(&proc->body[0]);
    REQUIRE(while_loop != nullptr);
}

TEST_CASE("WhileLoop: While loop with body statements", "[statements_sequential][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            process(clk)
                variable count : integer := 0;
            begin
                while count < 5 loop
                    y <= '1';
                end loop;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *proc = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    const auto *while_loop = std::get_if<ast::WhileLoop>(&proc->body[0]);
    REQUIRE(while_loop != nullptr);
    REQUIRE(while_loop->body.size() == 1);

    const auto *assign = std::get_if<ast::SequentialAssign>(&while_loop->body[0]);
    REQUIRE(assign != nullptr);
}
