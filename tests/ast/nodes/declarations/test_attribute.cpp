#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("AttributeDecl: Attribute declaration", "[declarations][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            attribute KEEP : boolean;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check attribute declaration when implemented
}

TEST_CASE("AttributeSpec: Attribute specification", "[declarations][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal my_signal : std_logic;
            attribute KEEP of my_signal : signal is true;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check attribute specification when implemented
}

TEST_CASE("AttributeDecl: Multiple attributes", "[declarations][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            attribute KEEP : boolean;
            attribute MARK_DEBUG : string;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple attributes when implemented
}

TEST_CASE("AttributeSpec: Attribute on type", "[declarations][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type MyType is range 0 to 255;
            attribute RANGE of MyType : type is 0 to 255;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check attribute on type when implemented
}
