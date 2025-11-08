#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("GenericClause: Single generic without default", "[clauses][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (WIDTH : integer);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    auto &generic = entity->generic_clause.generics[0];
    REQUIRE(generic.names[0] == "WIDTH");
    REQUIRE(generic.type_name == "integer");
    REQUIRE_FALSE(generic.default_expr.has_value());
}

TEST_CASE("GenericClause: Single generic with default", "[clauses][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (WIDTH : integer := 8);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    auto &generic = entity->generic_clause.generics[0];
    REQUIRE(generic.names.size() == 1);
    REQUIRE(generic.names[0] == "WIDTH");
    REQUIRE(generic.type_name == "integer");
    REQUIRE(generic.default_expr.has_value());
}

TEST_CASE("GenericClause: Multiple separate generic declarations", "[clauses][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (
                WIDTH : integer := 8;
                RESET_VAL : integer := 0;
                ENABLE_ASYNC : boolean := false
            );
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 3);

    REQUIRE(entity->generic_clause.generics[0].names[0] == "WIDTH");
    REQUIRE(entity->generic_clause.generics[0].type_name == "integer");

    REQUIRE(entity->generic_clause.generics[1].names[0] == "RESET_VAL");
    REQUIRE(entity->generic_clause.generics[1].type_name == "integer");

    REQUIRE(entity->generic_clause.generics[2].names[0] == "ENABLE_ASYNC");
    REQUIRE(entity->generic_clause.generics[2].type_name == "boolean");
}

TEST_CASE("GenericClause: Multiple generics same declaration", "[clauses][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (WIDTH, HEIGHT, DEPTH : integer := 8);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    auto &generic = entity->generic_clause.generics[0];
    REQUIRE(generic.names.size() == 3);
    REQUIRE(generic.names[0] == "WIDTH");
    REQUIRE(generic.names[1] == "HEIGHT");
    REQUIRE(generic.names[2] == "DEPTH");
    REQUIRE(generic.type_name == "integer");
    REQUIRE(generic.default_expr.has_value());
}

TEST_CASE("GenericClause: Generic with expression default", "[clauses][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (ADDR_WIDTH : integer := 2 ** 8);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    auto &generic = entity->generic_clause.generics[0];
    REQUIRE(generic.names[0] == "ADDR_WIDTH");
    REQUIRE(generic.default_expr.has_value());
}
