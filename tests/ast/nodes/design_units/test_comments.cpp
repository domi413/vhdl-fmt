#include "ast/node.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/test_utils.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

using test_utils::getComments;

TEST_CASE("Entity captures top-level leading comments", "[design_units][comments]")
{
    constexpr std::string_view VHDL_FILE = R"(
        -- License text
        -- Entity declaration for a simple counter
        entity MyEntity is end MyEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->trivia.has_value());

    const auto &trivia = entity->trivia.value();
    const auto texts = getComments(trivia.leading);

    REQUIRE(texts.size() == 2);
    REQUIRE(texts.front().contains("License text"));
    REQUIRE(texts.back().contains("Entity declaration"));
}

TEST_CASE("Generic captures both leading and inline comments", "[design_units][comments]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Example is
            generic (
                -- Leading for CONST_V
                CONST_V : integer := 42   -- Inline for CONST_V
            );
        end Example;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    const auto &g = entity->generic_clause.generics[0];
    REQUIRE(g.trivia.has_value());

    const auto &trivia = g.trivia.value();
    const auto lead = getComments(trivia.leading);
    const auto in = trivia.inline_comment->text;

    REQUIRE_FALSE(lead.empty());
    REQUIRE(lead.front().contains("Leading for CONST_V"));

    REQUIRE_FALSE(in.empty());
    REQUIRE(in.contains("Inline for CONST_V"));
}

TEST_CASE("Ports capture leading, trailing and inline comments", "[design_units][comments]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Example is
            port (
                -- leading clk
                clk : in std_logic;  -- inline clk
                -- trailing clk
                rst : in std_logic   -- inline rst
                -- trailing rst
            );
        end Example;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 2);

    const auto &clk = entity->port_clause.ports.front();
    const auto &clk_trivia = clk.trivia.value();
    const auto &clk_lead = getComments(clk_trivia.leading);
    const auto &clk_trail = getComments(clk_trivia.trailing);
    const auto &clk_inline = clk_trivia.inline_comment->text;

    REQUIRE(clk_lead.front().contains("leading clk"));
    REQUIRE(clk_trail.front().contains("trailing clk"));
    REQUIRE(clk_inline.contains("inline clk"));

    const auto &rst = entity->port_clause.ports.back();
    const auto &rst_trivia = rst.trivia.value();
    const auto &rst_lead = getComments(rst_trivia.leading);
    const auto &rst_trail = getComments(rst_trivia.trailing);
    const auto &rst_inline = rst_trivia.inline_comment->text;

    REQUIRE(rst_trail.front().contains("trailing rst"));
    REQUIRE(rst_inline.contains("inline rst"));
}
