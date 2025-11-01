#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("AliasDecl: Simple alias", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal LongSignalName : std_logic;
            alias ShortName is LongSignalName;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check alias declaration when implemented
}

TEST_CASE("AliasDecl: Object alias with slice", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
            alias byte : std_logic_vector(7 downto 0) is data(15 downto 8);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check object alias when implemented
}

TEST_CASE("AliasDecl: Non-object alias", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            alias sl is std_logic;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check non-object alias when implemented
}

TEST_CASE("AliasDecl: Alias for type", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            alias MyInt is integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type alias when implemented
}
