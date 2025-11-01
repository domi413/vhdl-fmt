#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("LibraryClause: Library declaration", "[declarations][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check library clause when implemented
}

TEST_CASE("LibraryClause: Multiple libraries", "[declarations][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        library work;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple libraries when implemented
}

TEST_CASE("LibraryClause: Library in package", "[declarations][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        package P is
            use IEEE.std_logic_1164.all;
            -- declarations
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check library in package when implemented
}

TEST_CASE("LibraryClause: Library with use in architecture", "[declarations][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        entity E is end E;
        architecture A of E is
            use IEEE.std_logic_1164.all;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check library with use in architecture when implemented
}
