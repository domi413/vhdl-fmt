#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("makeUseClause: Single use clause import", "[clauses][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use work.MyPackage.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check use clause when makeUseClause is implemented
}

TEST_CASE("makeUseClause: Multiple use clauses", "[clauses][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use work.Pkg1.all;
        use work.Pkg2.all;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check multiple use clauses when makeUseClause is implemented
}

TEST_CASE("makeUseClause: Use clause with specific items", "[clauses][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use IEEE.std_logic_1164.std_logic, IEEE.std_logic_1164.std_logic_vector;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check use clause with specific items when makeUseClause is implemented
}

TEST_CASE("makeUseClause: Use clause in architecture", "[clauses][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            use work.MyPackage.all;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check use clause in architecture when makeUseClause is implemented
}
