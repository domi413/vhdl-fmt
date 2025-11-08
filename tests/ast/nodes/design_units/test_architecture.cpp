#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ArchitectureBody: Basic architecture body", "[design_units][architecture]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of MyEntity is
            signal temp : std_logic;
        begin
            temp <= '1';
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *arch = std::get_if<ast::ArchitectureBody>(design.units.data());
    REQUIRE(arch != nullptr);
    REQUIRE(arch->name == "RTL");
    REQUIRE(arch->entity_name == "MyEntity");
    REQUIRE_FALSE(arch->decls.empty());
    REQUIRE_FALSE(arch->stmts.empty());
}

TEST_CASE("ArchitectureBody: Multiple architectures for same entity", "[design_units][architecture]")
{
    constexpr std::string_view VHDL_FILE = R"(
        architecture RTL of Counter is
        begin
        end RTL;

        architecture Behavioral of Counter is
        begin
        end Behavioral;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    auto *arch1 = std::get_if<ast::ArchitectureBody>(design.units.data());
    REQUIRE(arch1 != nullptr);
    REQUIRE(arch1->name == "RTL");
    REQUIRE(arch1->entity_name == "Counter");

    auto *arch2 = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch2 != nullptr);
    REQUIRE(arch2->name == "Behavioral");
    REQUIRE(arch2->entity_name == "Counter");
}
