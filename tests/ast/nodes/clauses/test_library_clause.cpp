#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("LibraryClause: Single library declaration", "[clauses][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check library clause when makeLibraryClause is implemented
}

TEST_CASE("LibraryClause: Multiple libraries", "[clauses][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        library work;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check multiple libraries when makeLibraryClause is implemented
}

TEST_CASE("LibraryClause: Library in package", "[clauses][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        package P is
            use IEEE.std_logic_1164.all;
            -- declarations
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check library in package when makeLibraryClause is implemented
}

TEST_CASE("LibraryClause: Library with use in architecture", "[clauses][library]")
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
    // TODO: Check library with use in architecture when makeLibraryClause is implemented
}
