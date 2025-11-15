#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
TEST_CASE("WaitStatement: Simple wait statement", "[statements_sequential][wait_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
        architecture A of E is
        begin
            process
            begin
                wait;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto& arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto& proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);

    // Check if wait statement exists in body
    REQUIRE_FALSE(proc.body.empty());
}

TEST_CASE("WaitStatement: Wait until with condition", "[statements_sequential][wait_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
        architecture A of E is
        begin
            process
            begin
                wait until clk = '1';
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto& arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto& proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);
}

TEST_CASE("WaitStatement: Wait on sensitivity list", "[statements_sequential][wait_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk, reset : in std_logic);
        end E;
        architecture A of E is
        begin
            process
            begin
                wait on clk, reset;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto& arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto& proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);
}
