#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("PostponedProcess: Simple postponed process", "[statements][postponed_process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            postponed process
            begin
                -- statements
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check postponed process when implemented
}

TEST_CASE("PostponedProcess: Postponed process with sensitivity list",
          "[statements][postponed_process]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            postponed process (clk)
            begin
                if rising_edge(clk) then
                    count <= count + 1;
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check postponed process when implemented
}
