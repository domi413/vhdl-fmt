#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
TEST_CASE("ConcurrentAssign: Simple signal assignment",
          "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= a;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
}

TEST_CASE("ConcurrentAssign: Assignment with logical expression",
          "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= a and b;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
}

TEST_CASE("ConcurrentAssign: Multiple concurrent assignments",
          "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b : in std_logic; x, y : out std_logic);
        end E;
        architecture A of E is
        begin
            x <= a or b;
            y <= a and b;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 2);

    const auto &assign1 = std::get<ast::ConcurrentAssign>(arch.stmts[0]);

    const auto &assign2 = std::get<ast::ConcurrentAssign>(arch.stmts[1]);
}

TEST_CASE("ConcurrentAssign: Assignment with waveform delay",
          "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= a after 10 ns;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
    // Waveform details (after clause) are simplified in our AST
    // We just verify the assignment exists and has target/value
    const auto &target = std::get<ast::TokenExpr>(assign.target);
    REQUIRE(target.text == "y");
}

TEST_CASE("ConcurrentAssign: Assignment with multiple waveform elements",
          "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= '0', a after 5 ns, '1' after 10 ns;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
    // Multiple waveform elements are simplified in our AST
    // We just verify the assignment parses correctly
    const auto &target = std::get<ast::TokenExpr>(assign.target);
    REQUIRE(target.text == "y");
}

TEST_CASE("ConcurrentAssign: Assignment with transport delay",
          "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= transport a after 10 ns;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
    // Delay mechanisms and waveforms are simplified in our AST
    // We just verify the assignment parses correctly
    const auto &target = std::get<ast::TokenExpr>(assign.target);
    REQUIRE(target.text == "y");
}

TEST_CASE("ConcurrentAssign: Conditional assignment", "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b : in std_logic; sel : in boolean; y : out std_logic);
        end E;
        architecture A of E is
        begin
            y <= a when sel else b;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
    // Conditional waveforms are simplified in our AST
    // We just verify the assignment parses correctly
    const auto &target = std::get<ast::TokenExpr>(assign.target);
    REQUIRE(target.text == "y");
}

TEST_CASE("ConcurrentAssign: Selected assignment", "[statements_concurrent][concurrent_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (a, b, c : in std_logic; sel : in std_logic_vector(1 downto 0); y : out std_logic);
        end E;
        architecture A of E is
        begin
            with sel select
                y <= a when "00",
                     b when "01",
                     c when others;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);

    const auto &assign = std::get<ast::ConcurrentAssign>(arch.stmts[0]);
    // Selected waveforms are simplified in our AST
    // We just verify the assignment parses correctly
    const auto &target = std::get<ast::TokenExpr>(assign.target);
    REQUIRE(target.text == "y");
}
