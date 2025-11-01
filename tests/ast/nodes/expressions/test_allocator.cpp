#include "ast/nodes/expressions.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>
TEST_CASE("Allocator: New with qualified expression", "[expressions][allocator]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable ptr : access integer := new integer'(42);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check allocator with qualified expression when implemented
}

TEST_CASE("Allocator: New with subtype", "[expressions][allocator]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            subtype SmallInt is integer range 0 to 255;
        begin
            process
                variable ptr : access SmallInt := new SmallInt'(100);
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check allocator with subtype when implemented
}

TEST_CASE("Allocator: New for record", "[expressions][allocator]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type MyRecord is record
                a : integer;
                b : std_logic;
            end record;
        begin
            process
                variable ptr : access MyRecord := new MyRecord'(a => 1, b => '0');
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check allocator for record when implemented
}

TEST_CASE("Allocator: New without initial value", "[expressions][allocator]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable ptr : access integer := new integer;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check allocator without initial value when implemented
}
