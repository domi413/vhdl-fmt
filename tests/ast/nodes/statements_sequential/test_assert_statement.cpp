#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
TEST_CASE("AssertStatement: Simple assert without message",
          "[statements_sequential][assert_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : in std_logic);
        end E;
        architecture A of E is
        begin
            process(data)
            begin
                assert data = '1';
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);
}

TEST_CASE("AssertStatement: Assert with report message",
          "[statements_sequential][assert_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (valid : in boolean);
        end E;
        architecture A of E is
        begin
            process(valid)
            begin
                assert valid
                    report "Validation failed"
                    severity error;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);
}

TEST_CASE("AssertStatement: Assert with complex condition",
          "[statements_sequential][assert_statement]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk, reset : in std_logic);
        end E;
        architecture A of E is
        begin
            process(clk, reset)
            begin
                assert (clk = '1' and reset = '0')
                    report "Invalid clock/reset combination"
                    severity warning;
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &proc = std::get<ast::Process>(arch.stmts[0]);
    REQUIRE(proc.body.size() == 1);
}
