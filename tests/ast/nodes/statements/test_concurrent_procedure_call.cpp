#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("ConcurrentProcedureCall: Simple concurrent procedure call",
          "[statements][concurrent_procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            MyConcurrentProc;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check concurrent procedure call when implemented
}

TEST_CASE("ConcurrentProcedureCall: Concurrent procedure call with parameters",
          "[statements][concurrent_procedure_call]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            MyConcurrentProc(input_a, output_b);
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check concurrent procedure call when implemented
}
