#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("TypeDecl: Enumeration type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type StateType is (IDLE, RUNNING, DONE);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type declaration when implemented
}

TEST_CASE("TypeDecl: Record type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type RecordType is record
                field1 : integer;
                field2 : std_logic;
            end record;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type declaration when implemented
}

TEST_CASE("TypeDecl: Array type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type ByteArray is array (0 to 15) of std_logic_vector(7 downto 0);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type declaration when implemented
}

TEST_CASE("TypeDecl: Integer type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type MyInt is range 0 to 100;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check integer type declaration when implemented
}
