#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("SelectedAssign: Simple selected assignment", "[statements][selected_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            with selector select
                output <= val1 when "00",
                          val2 when "01",
                          'Z' when others;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check selected assign statement when implemented
}

TEST_CASE("SelectedAssign: Selected assignment with ranges", "[statements][selected_assign]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            with selector select
                output <= "0000" when 0 to 3,
                          "1111" when 4 to 7,
                          "ZZZZ" when others;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check selected assign statement when implemented
}
