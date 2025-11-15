#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
TEST_CASE("AliasDecl: Simple signal alias", "[declarations][alias_decl]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
            alias byte_data : std_logic_vector(7 downto 0) is data;
        begin
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.decls.size() == 2);
}

TEST_CASE("AliasDecl: Alias for bit slice", "[declarations][alias_decl]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
            alias high_byte : std_logic_vector(7 downto 0) is data(15 downto 8);
            alias low_byte  : std_logic_vector(7 downto 0) is data(7 downto 0);
        begin
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.decls.size() == 3);
}

TEST_CASE("AliasDecl: Alias with different subtype", "[declarations][alias_decl]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
        end E;
        architecture A of E is
            signal counter : integer;
            alias count_value : natural is counter;
        begin
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto &arch = std::get<ast::Architecture>(design.units[1]);
    REQUIRE(arch.decls.size() == 2);
}
