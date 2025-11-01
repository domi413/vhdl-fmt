#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("LoopWithLabel: Simple labeled loop", "[statements][loop_with_label]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                my_loop : loop
                    exit my_loop when done;
                end loop my_loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check loop with label when implemented
}

TEST_CASE("LoopWithLabel: Nested labeled loops", "[statements][loop_with_label]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                outer : loop
                    inner : loop
                        exit outer when done;
                    end loop inner;
                end loop outer;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check loop with label when implemented
}
