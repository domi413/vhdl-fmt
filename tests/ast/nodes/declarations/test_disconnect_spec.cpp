#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("DisconnectSpec: Disconnect specification", "[declarations][disconnect]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            disconnect signal_name : type_mark after time_expression;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check disconnect specification when implemented
}

TEST_CASE("DisconnectSpec: Disconnect with specific time", "[declarations][disconnect]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal my_sig : std_logic;
            disconnect my_sig : std_logic after 10 ns;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check disconnect with time when implemented
}

TEST_CASE("DisconnectSpec: Disconnect for multiple signals", "[declarations][disconnect]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal sig1, sig2 : std_logic;
            disconnect sig1, sig2 : std_logic after 5 ns;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check disconnect for multiple signals when implemented
}

TEST_CASE("DisconnectSpec: Disconnect in guarded block", "[declarations][disconnect]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            block
                signal int_sig : std_logic;
            begin
                disconnect int_sig : std_logic after 1 us;
            end block;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check disconnect in block when implemented
}
