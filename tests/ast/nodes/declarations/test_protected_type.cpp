#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("ProtectedType: Protected type declaration", "[declarations][protected_type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            protected type SharedData is
                -- declarations
            end protected type;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check protected type declaration when implemented
}

TEST_CASE("ProtectedType: Protected type with methods", "[declarations][protected_type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            protected type Counter is
                procedure increment;
                impure function get_value return integer;
            end protected type;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check protected type with methods when implemented
}

TEST_CASE("ProtectedTypeBody: Protected type body", "[declarations][protected_type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            protected body SharedData is
                -- implementations
            end protected body;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check protected type body when implemented
}

TEST_CASE("ProtectedTypeBody: Protected type body implementation", "[declarations][protected_type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            protected body Counter is
                variable value : integer := 0;
                procedure increment is
                begin
                    value := value + 1;
                end procedure;
                impure function get_value return integer is
                begin
                    return value;
                end function;
            end protected body;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check protected type body when implemented
}
