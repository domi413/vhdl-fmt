#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("SliceExpr: Simple slice downto", "[expressions][slice]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
        begin
            process
                variable lower : std_logic_vector(7 downto 0) := data(7 downto 0);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check slice expression when implemented
}

TEST_CASE("SliceExpr: Slice to", "[expressions][slice]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(0 to 15);
        begin
            process
                variable upper : std_logic_vector(8 to 15) := data(8 to 15);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check slice to when implemented
}

TEST_CASE("SliceExpr: Slice with variables", "[expressions][slice]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
        begin
            process
                variable start, end_idx : integer := 7;
                variable slice : std_logic_vector(7 downto 0) := data(start downto end_idx);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check slice with variables when implemented
}

TEST_CASE("SliceExpr: Slice in assignment", "[expressions][slice]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
            signal output : std_logic_vector(3 downto 0);
        begin
            output <= data(11 downto 8);
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check slice in assignment when implemented
}
