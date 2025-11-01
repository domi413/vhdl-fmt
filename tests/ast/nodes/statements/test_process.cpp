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
