#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ExternalName: External signal reference", "[expressions][external_name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable val : std_logic := <<signal .testbench.dut.internal_signal : std_logic>>;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check external signal reference when implemented
}

TEST_CASE("ExternalName: External constant reference", "[expressions][external_name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable val : integer := <<constant .testbench.dut.max_value : integer>>;
            begin
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check external constant reference when implemented
}

TEST_CASE("ExternalName: External variable reference", "[expressions][external_name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable local_val : integer;
            begin
                local_val := <<variable .testbench.dut.shared_var : integer>>;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check external variable reference when implemented
}

TEST_CASE("ExternalName: External in assignment", "[expressions][external_name]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            <<signal .testbench.dut.output_signal : std_logic>> <= '1';
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check external name in assignment when implemented
}
