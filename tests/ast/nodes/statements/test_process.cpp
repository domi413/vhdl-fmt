#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Process: Process with sensitivity list", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process (clk, rst)
            begin
                if rst = '1' then
                    count <= 0;
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->sensitivity_list.size() == 2);
    REQUIRE(proc->sensitivity_list[0] == "clk");
    REQUIRE(proc->sensitivity_list[1] == "rst");
}

TEST_CASE("Process: Process with single signal in sensitivity list", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process (clk)
            begin
                count <= count + 1;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->sensitivity_list.size() == 1);
    REQUIRE(proc->sensitivity_list[0] == "clk");
}

TEST_CASE("Process: Process without sensitivity list", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                wait until clk = '1';
                count <= count + 1;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->sensitivity_list.empty());
}

TEST_CASE("Process: Labeled process", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            counter_proc : process (clk)
            begin
                count <= count + 1;
            end process counter_proc;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->label.has_value());
    REQUIRE(proc->label.value() == "counter_proc");
}

TEST_CASE("Process: Process with multiple statements", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process (clk, rst)
            begin
                if rst = '1' then
                    count <= 0;
                    valid <= '0';
                elsif rising_edge(clk) then
                    count <= count + 1;
                    valid <= '1';
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->sensitivity_list.size() == 2);
    REQUIRE_FALSE(proc->body.empty());
}

TEST_CASE("Process: Multiple processes in architecture", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            proc1 : process (clk)
            begin
                counter <= counter + 1;
            end process proc1;

            proc2 : process (rst)
            begin
                if rst = '1' then
                    counter <= 0;
                end if;
            end process proc2;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 2);

    auto *proc1 = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc1 != nullptr);
    REQUIRE(proc1->label.has_value());
    REQUIRE(proc1->label.value() == "proc1");

    auto *proc2 = std::get_if<ast::Process>(&arch->stmts[1]);
    REQUIRE(proc2 != nullptr);
    REQUIRE(proc2->label.has_value());
    REQUIRE(proc2->label.value() == "proc2");
}

TEST_CASE("Process: Combinational process", "[statements][process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process (a, b, c)
            begin
                result := a and b or c;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE(proc->sensitivity_list.size() == 3);
    REQUIRE(proc->sensitivity_list[0] == "a");
    REQUIRE(proc->sensitivity_list[1] == "b");
    REQUIRE(proc->sensitivity_list[2] == "c");
}

// Postponed process (VHDL-93 feature, not yet implemented)
// TEST_CASE("Process: Postponed process", "[statements][process]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity Test is end Test;
//         architecture RTL of Test is
//         begin
//             postponed process (clk)
//             begin
//                 -- Executes at end of delta cycle
//                 check_signal <= data;
//             end process;
//         end RTL;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check postponed process when implemented
// }
