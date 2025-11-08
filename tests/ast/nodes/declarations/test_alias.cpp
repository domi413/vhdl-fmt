#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("makeAliasDecl: Simple alias", "[declarations][alias]")
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
    REQUIRE(design.units.size() == 2);

    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);

    auto *alias = std::get_if<ast::AliasDecl>(&arch->decls[1]);
    REQUIRE(alias != nullptr);
    REQUIRE(alias->name == "ShortName");
    REQUIRE_FALSE(alias->type_indication.has_value());
    REQUIRE_FALSE(alias->signature.has_value());

    auto *target = std::get_if<ast::TokenExpr>(&alias->target);
    REQUIRE(target != nullptr);
    REQUIRE(target->text == "LongSignalName");
}

TEST_CASE("makeAliasDecl: Object alias with slice", "[declarations][alias]")
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
    REQUIRE(design.units.size() == 2);

    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);

    auto *alias = std::get_if<ast::AliasDecl>(&arch->decls[1]);
    REQUIRE(alias != nullptr);
    REQUIRE(alias->name == "byte");
    REQUIRE(alias->type_indication.has_value());
    REQUIRE_FALSE(alias->signature.has_value());

    auto *call_expr = std::get_if<ast::FunctionCallOrIndexedNamePart>(&alias->target);
    REQUIRE(call_expr != nullptr);
}

TEST_CASE("makeAliasDecl: Alias with subtype indication", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal status : integer;
            alias error_code : integer is status;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);

    auto *alias = std::get_if<ast::AliasDecl>(&arch->decls[1]);
    REQUIRE(alias != nullptr);
    REQUIRE(alias->name == "error_code");
    REQUIRE(alias->type_indication.has_value());
    REQUIRE(alias->type_indication.value() == "integer");

    auto *target = std::get_if<ast::TokenExpr>(&alias->target);
    REQUIRE(target != nullptr);
    REQUIRE(target->text == "status");
}
