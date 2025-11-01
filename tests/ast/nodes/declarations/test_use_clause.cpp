#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("UseClause: Use clause import", "[declarations][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use work.MyPackage.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check use clause when implemented
}

TEST_CASE("UseClause: Multiple use clauses", "[declarations][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use work.Pkg1.all;
        use work.Pkg2.all;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple use clauses when implemented
}

TEST_CASE("UseClause: Use clause with specific items", "[declarations][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use IEEE.std_logic_1164.std_logic, IEEE.std_logic_1164.std_logic_vector;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check use clause with specific items when implemented
}

TEST_CASE("UseClause: Use clause in architecture", "[declarations][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            use work.MyPackage.all;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check use clause in architecture when implemented
}
