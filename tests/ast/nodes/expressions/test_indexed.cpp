#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("IndexedExpr: Single index", "[expressions][indexed]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            process
                variable bit_val : std_logic := data(0);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check indexed expression when implemented
}

TEST_CASE("IndexedExpr: Multiple indices", "[expressions][indexed]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type Matrix is array (0 to 3, 0 to 3) of integer;
            signal mat : Matrix;
        begin
            process
                variable val : integer := mat(1, 2);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple indices when implemented
}

TEST_CASE("IndexedExpr: Index with expression", "[expressions][indexed]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
        begin
            process
                variable idx : integer := 5;
                variable bit_val : std_logic := data(idx + 3);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check index with expression when implemented
}

TEST_CASE("IndexedExpr: Index in assignment", "[expressions][indexed]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
            data(4) <= '1';
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check index in assignment when implemented
}
