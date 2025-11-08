#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("SubtypeDecl: Constrained subtype", "[declarations][subtype]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            subtype SmallInt is integer range 0 to 255;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check subtype declaration when implemented
}

TEST_CASE("SubtypeDecl: Subtype with resolution function", "[declarations][subtype]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            subtype ResolvedStdLogic is std_logic;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check subtype with resolution when implemented
}

TEST_CASE("SubtypeDecl: Unconstrained subtype", "[declarations][subtype]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            subtype MyVector is std_logic_vector;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check unconstrained subtype when implemented
}
