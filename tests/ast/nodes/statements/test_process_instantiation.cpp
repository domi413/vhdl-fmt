#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ProcessInstantiation: Simple process instantiation",
          "[statements][process_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            proc_inst : process (clk)
            begin
                if rising_edge(clk) then
                    count <= count + 1;
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check process instantiation when implemented
}

TEST_CASE("ProcessInstantiation: Process instantiation with declarations",
          "[statements][process_instantiation]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            proc_inst : process (clk, rst) is
                variable temp : integer;
            begin
                if rst = '1' then
                    temp := 0;
                elsif rising_edge(clk) then
                    temp := temp + 1;
                end if;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check process instantiation when implemented
}
