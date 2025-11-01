#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("AccessDeref: Dereference access type", "[expressions][access_deref]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable ptr : access integer := new integer'(42);
                variable val : integer := ptr.all;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check dereference when implemented
}

TEST_CASE("AccessDeref: Dereference in assignment", "[expressions][access_deref]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable ptr : access integer := new integer;
            begin
                ptr.all := 100;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check dereference in assignment when implemented
}

TEST_CASE("AccessDeref: Dereference record access", "[expressions][access_deref]")
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
                variable val : integer := ptr.all.a;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check dereference record access when implemented
}

TEST_CASE("AccessDeref: Dereference in expression", "[expressions][access_deref]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable ptr : access integer := new integer'(5);
                variable result : integer := ptr.all + 10;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check dereference in expression when implemented
}
