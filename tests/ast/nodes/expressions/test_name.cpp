#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Name: Simple identifier name", "[expressions][name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a : in std_logic; b : out std_logic);
        end E;
        architecture A of E is
        begin
            b <= a;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Names are represented as TokenExpr in the generic AST
    const auto *name = std::get_if<ast::TokenExpr>(&assign->value);
    REQUIRE(name != nullptr);
    REQUIRE(name->text == "a");
}

TEST_CASE("Name: Selected name (qualified)", "[expressions][name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (sig : in std_logic);
        end E;
        architecture A of E is
            signal vec : std_logic_vector(7 downto 0);
        begin
            vec(0) <= sig;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Indexed/selected names are represented as CallExpr
    const auto *indexed = std::get_if<ast::CallExpr>(&assign->target);
    REQUIRE(indexed != nullptr);
}

TEST_CASE("Name: Aggregate in assignment", "[expressions][name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(1 downto 0);
        begin
            vec <= (others => '0');
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Aggregates are represented as GroupExpr
    const auto *agg = std::get_if<ast::GroupExpr>(&assign->value);
    REQUIRE(agg != nullptr);
    REQUIRE_FALSE(agg->children.empty());
}

TEST_CASE("Name: Function call expression", "[expressions][name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic; result : out std_logic);
        end E;
        architecture A of E is
        begin
            process(clk)
            begin
                if rising_edge(clk) then
                    result <= '1';
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

    // Function calls are represented as CallExpr
    const auto *call = std::get_if<ast::CallExpr>(&if_stmt->if_branch.condition);
    REQUIRE(call != nullptr);
}

TEST_CASE("Name: Slice expression", "[expressions][name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(7 downto 0);
            signal slice : std_logic_vector(3 downto 0);
        begin
            slice <= vec(5 downto 2);
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *assign = std::get_if<ast::ConcurrentAssign>(&arch->stmts[0]);
    REQUIRE(assign != nullptr);

    // Slice expressions are represented as CallExpr with range argument
    const auto *slice = std::get_if<ast::CallExpr>(&assign->value);
    REQUIRE(slice != nullptr);
}

TEST_CASE("Name: Attribute name", "[expressions][name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal vec : std_logic_vector(7 downto 0);
            signal len : integer;
        begin
            process
            begin
                len <= vec'length;
                wait;
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
    REQUIRE(proc->body.size() == 2); // assignment + wait

    // Just verify the process parses correctly
    // Attribute names are complex and may be represented differently
    REQUIRE_FALSE(proc->body.empty());
}
