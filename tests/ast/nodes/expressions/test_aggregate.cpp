#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("Aggregate: Named aggregate for record", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type RecordType is record
                field1 : integer;
                field2 : std_logic;
            end record;
            signal rec : RecordType := (field1 => 1, field2 => '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check named aggregate when implemented
}

TEST_CASE("Aggregate: Positional aggregate for array", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(3 downto 0) := (0, 1, 2, 3);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check positional aggregate when implemented
}

TEST_CASE("Aggregate: Mixed named and positional", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type RecordType is record
                a, b, c : integer;
            end record;
            signal rec : RecordType := (1, b => 2, 3);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check mixed aggregate when implemented
}

TEST_CASE("Aggregate: Aggregate with others", "[expressions][aggregate]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0) := (0 => '1', others => '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check aggregate with others when implemented
}
