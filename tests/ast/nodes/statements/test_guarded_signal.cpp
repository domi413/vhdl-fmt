#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("GuardedSignalAssignment: Simple guarded signal assignment",
          "[statements][guarded_signal_assignment]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            output <= guarded input;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check guarded signal assignment when implemented
}

TEST_CASE("GuardedSignalAssignment: Guarded assignment with delay",
          "[statements][guarded_signal_assignment]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            output <= guarded input after 5 ns;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check guarded signal assignment when implemented
}
