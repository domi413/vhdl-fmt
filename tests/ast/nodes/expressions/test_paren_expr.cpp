#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("ParenExpr: Simple parenthesized expression", "[expressions][paren]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := (5 + 3);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check parenthesized expression when implemented
}

TEST_CASE("ParenExpr: Nested parentheses", "[expressions][paren]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := ((5 + 3) * 2);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check nested parentheses when implemented
}

TEST_CASE("ParenExpr: Parentheses in function call", "[expressions][paren]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable result : integer := func((a + b));
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check parentheses in function call when implemented
}

TEST_CASE("ParenExpr: Parentheses around aggregate", "[expressions][paren]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(3 downto 0) := ('1', '0', '1', '0');
        begin
            process
                variable temp : std_logic_vector(3 downto 0) := (data);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check parentheses around aggregate when implemented
}
