#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("GroupExpr: Aggregate with others", "[expressions][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0) := (others => '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check group expression when implemented
}

TEST_CASE("GroupExpr: Aggregate with positional elements", "[expressions][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(3 downto 0) := ('1', '0', '1', '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check positional aggregate when implemented
}

TEST_CASE("GroupExpr: Parenthesized expression", "[expressions][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := (5 + 3) * 2;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check parenthesized expression when implemented
}

TEST_CASE("GroupExpr: Nested aggregates", "[expressions][group]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type Matrix is array (0 to 1, 0 to 1) of integer;
            signal mat : Matrix := ((1, 2), (3, 4));
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check nested aggregates when implemented
}
