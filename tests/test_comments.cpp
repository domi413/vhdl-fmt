#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

using test_utils::buildAstFromSource;
using test_utils::leadingComments;
using test_utils::trailingComments;

// -----------------------------------------------------------------------------
// Entity-level leading comments
// -----------------------------------------------------------------------------
TEST_CASE("Entity captures top-level leading comments", "[comments][entity]")
{
    const std::string vhdl = R"(
        -- License text
        -- Entity declaration for a simple counter
        entity MyEntity is end MyEntity;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = std::get_if<ast::Entity>(design->units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->trivia.has_value());

    const auto texts = leadingComments(entity->trivia->leading);

    REQUIRE(texts.size() == 2);
    REQUIRE(texts.front().contains("License text"));
    REQUIRE(texts.back().contains("Entity declaration"));
}

// -----------------------------------------------------------------------------
// Generic-level leading and inline comments
// -----------------------------------------------------------------------------
TEST_CASE("Generic captures both leading and inline comments", "[comments][generic]")
{
    const std::string vhdl = R"(
        entity Example is
            generic (
                -- Leading for CONST_V
                CONST_V : integer := 42   -- Inline for CONST_V
            );
        end Example;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = std::get_if<ast::Entity>(design->units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    const auto &g = entity->generic_clause.generics[0];
    REQUIRE(g.trivia.has_value());

    const auto lead = leadingComments(g.trivia->leading);
    const auto trail = trailingComments(g.trivia->trailing);

    REQUIRE_FALSE(lead.empty());
    REQUIRE(lead.front().contains("Leading for CONST_V"));

    REQUIRE_FALSE(trail.empty());
    REQUIRE(trail.front().contains("Inline for CONST_V"));
}

// -----------------------------------------------------------------------------
// Port-level leading and inline comments
// -----------------------------------------------------------------------------
TEST_CASE("Ports capture leading and inline comments", "[comments][ports]")
{
    const std::string vhdl = R"(
        entity Example is
            port (
                -- Clock input
                clk : in std_logic;  -- inline clock
                -- Reset input
                rst : in std_logic   -- inline reset
            );
        end Example;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = std::get_if<ast::Entity>(design->units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 2);

    {
        const auto &clk = entity->port_clause.ports[0];
        REQUIRE(clk.trivia.has_value());
        const auto lead = leadingComments(clk.trivia->leading);
        const auto trail = trailingComments(clk.trivia->trailing);
        REQUIRE_FALSE(lead.empty());
        REQUIRE(lead.front().contains("Clock input"));
        REQUIRE_FALSE(trail.empty());
        REQUIRE(trail.front().contains("inline clock"));
    }
    {
        const auto &rst = entity->port_clause.ports[1];
        REQUIRE(rst.trivia.has_value());
        const auto lead = leadingComments(rst.trivia->leading);
        const auto trail = trailingComments(rst.trivia->trailing);
        REQUIRE_FALSE(lead.empty());
        REQUIRE(lead.front().contains("Reset input"));
        REQUIRE_FALSE(trail.empty());
        REQUIRE(trail.front().contains("inline reset"));
    }
}
