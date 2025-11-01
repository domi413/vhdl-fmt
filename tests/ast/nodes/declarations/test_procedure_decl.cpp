#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ProcedureDecl: Procedure prototype", "[declarations][procedure]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            procedure Reset(signal clk : in std_logic);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure declaration when implemented
}

TEST_CASE("ProcedureBody: Procedure implementation", "[declarations][procedure]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            procedure Reset(signal clk : in std_logic) is
            begin
                wait until rising_edge(clk);
            end procedure Reset;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure body when implemented
}

TEST_CASE("ProcedureDecl: Procedure with multiple parameters", "[declarations][procedure]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            procedure Init(signal a, b : in std_logic; signal c : out std_logic);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure with multiple params when implemented
}

TEST_CASE("ProcedureBody: Procedure with body", "[declarations][procedure]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            procedure DoSomething(signal s : in std_logic) is
            begin
                if s = '1' then
                    -- do something
                end if;
            end procedure DoSomething;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure body when implemented
}
