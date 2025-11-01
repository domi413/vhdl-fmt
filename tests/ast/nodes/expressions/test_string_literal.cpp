#include "ast/nodes/expressions.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>
TEST_CASE("StringLiteral: String literal", "[expressions][string_literal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable msg : string(1 to 5) := "hello";
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check string literal when implemented
}

TEST_CASE("StringLiteral: Bit string hex", "[expressions][string_literal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0) := X"FF";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check bit string hex when implemented
}

TEST_CASE("StringLiteral: Bit string octal", "[expressions][string_literal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(8 downto 0) := O"777";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check bit string octal when implemented
}

TEST_CASE("StringLiteral: Bit string binary", "[expressions][string_literal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(3 downto 0) := B"1010";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check bit string binary when implemented
}
