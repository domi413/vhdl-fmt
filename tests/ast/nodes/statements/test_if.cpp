#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

#include <variant>
TEST_CASE("IfStatement: Simple if statement", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if condition then
                    result := '1';
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
    REQUIRE(if_stmt->elsif_branches.empty());
    REQUIRE_FALSE(if_stmt->else_branch.has_value());
}

TEST_CASE("IfStatement: If-else statement", "[statements][if]")
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
    REQUIRE(if_stmt->elsif_branches.empty());
    REQUIRE(if_stmt->else_branch.has_value());
}

TEST_CASE("IfStatement: If-elsif-else statement", "[statements][if]")
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

TEST_CASE("IfStatement: Multiple elsif branches", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if state = IDLE then
                    next_state := RUNNING;
                elsif state = RUNNING then
                    next_state := PAUSE;
                elsif state = PAUSE then
                    next_state := DONE;
                elsif state = DONE then
                    next_state := IDLE;
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
    REQUIRE(if_stmt->elsif_branches.size() == 3);
    REQUIRE_FALSE(if_stmt->else_branch.has_value());
}

TEST_CASE("IfStatement: If with comparison", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if count < 10 then
                    count := count + 1;
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
}

TEST_CASE("IfStatement: If with logical operators", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if enable = '1' and ready = '1' then
                    data_out := data_in;
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
}

TEST_CASE("IfStatement: Nested if statements", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                if outer_condition then
                    if inner_condition then
                        result := '1';
                    else
                        result := '0';
                    end if;
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
}

TEST_CASE("IfStatement: Rising edge condition", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process (clk)
            begin
                if rising_edge(clk) then
                    count <= count + 1;
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
}

TEST_CASE("IfStatement: Reset condition pattern", "[statements][if]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process (clk, rst)
            begin
                if rst = '1' then
                    count <= 0;
                elsif rising_edge(clk) then
                    count <= count + 1;
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
}
