#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
TEST_CASE("SelectedAssign: Simple with statement", "[statements_concurrent][selected_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (sel : in std_logic; y : out std_logic);
        end E;
        architecture A of E is
        begin
            with sel select
                y <= '0' when '0',
                     '1' when '1',
                     'X' when others;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);
}

TEST_CASE("SelectedAssign: With statement using integer selector",
          "[statements_concurrent][selected_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (sel : in integer; y : out std_logic_vector(1 downto 0));
        end E;
        architecture A of E is
        begin
            with sel select
                y <= "00" when 0,
                     "01" when 1,
                     "10" when 2,
                     "11" when 3,
                     "XX" when others;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);
}

TEST_CASE("SelectedAssign: With statement using range", "[statements_concurrent][selected_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (sel : in integer; y : out std_logic);
        end E;
        architecture A of E is
        begin
            with sel select
                y <= '0' when 0 to 5,
                     '1' when 6 to 10,
                     'X' when others;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.stmts.size() == 1);
}
