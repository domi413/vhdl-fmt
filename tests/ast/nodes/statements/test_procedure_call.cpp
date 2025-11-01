#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("ProcedureCall: Simple procedure call", "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                Reset;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure call statement when implemented
}

TEST_CASE("ProcedureCall: Procedure call with parameters", "[statements][procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                Reset(clk);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure call statement when implemented
}
