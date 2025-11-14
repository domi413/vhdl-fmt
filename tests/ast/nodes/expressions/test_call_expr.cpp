#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("CallExpr: Function call expression", "[expressions][call_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic; q : out std_logic);
        end E;
        architecture A of E is
        begin
            process(clk)
            begin
                if rising_edge(clk) then
                    q <= '1';
                end if;
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

    const auto *if_stmt = std::get_if<ast::IfStatement>(&proc->body[0]);
    REQUIRE(if_stmt != nullptr);

    const auto *call = std::get_if<ast::CallExpr>(&if_stmt->if_branch.condition);
    REQUIRE(call != nullptr);
}

TEST_CASE("CallExpr: Array indexing as call expression", "[expressions][call_expr]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : in std_logic_vector(7 downto 0); bit_out : out std_logic);
        end E;
        architecture A of E is
        begin
            bit_out <= data(3);
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    const auto *call = std::get_if<ast::CallExpr>(&assign->value);
    REQUIRE(call != nullptr);
}
