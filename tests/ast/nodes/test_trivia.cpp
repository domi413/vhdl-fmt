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

    // Change: Check pointer instead of optional
    REQUIRE(entity->hasTrivia());

    // Change: Use convenience method to get span
    const auto texts = getComments(entity->getLeading());

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

    // Change: Check pointer
    REQUIRE(g.trivia != nullptr);

    // Change: Use convenience methods
    const auto lead = getComments(g.getLeading());
    const auto in = g.getInlineComment(); // Returns optional<string_view>

    REQUIRE_FALSE(lead.empty());
    REQUIRE(lead.front().contains("Leading for CONST_V"));

    REQUIRE(in.has_value());
    REQUIRE(in->contains("Inline for CONST_V"));
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

    // Change: Simplify with convenience methods
    const auto clk_lead = getComments(clk.getLeading());
    const auto clk_trail = getComments(clk.getTrailing());
    const auto clk_inline = clk.getInlineComment();

    REQUIRE(clk_lead.front().contains("leading clk"));
    REQUIRE(clk_trail.front().contains("trailing clk"));
    REQUIRE(clk_inline.value_or("").contains("inline clk"));

    const auto &rst = entity->port_clause.ports.back();

    const auto rst_lead = getComments(rst.getLeading());
    const auto rst_trail = getComments(rst.getTrailing());
    const auto rst_inline = rst.getInlineComment();

    REQUIRE(rst_trail.front().contains("trailing rst"));
    REQUIRE(rst_inline.value_or("").contains("inline rst"));
}

TEST_CASE("Generic captures paragraph breaks after comments with blank lines",
          "[design_units][comments]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity ExampleEntity is
            generic (
                one : integer := 1;

                -- test

                two : integer := 2
            );
        end ExampleEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 2);

    const auto &one = entity->generic_clause.generics[0];
    const auto &two = entity->generic_clause.generics[1];

    // Change: Check pointer
    REQUIRE(one.trivia != nullptr);

    // Access trailing via span
    const auto trailing = one.getTrailing();

    // Should have 3 elements: ParagraphBreak, Comment, ParagraphBreak
    REQUIRE(trailing.size() == 3);

    // First should be a paragraph break (blank line before comment)
    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(trailing[0]));
    const auto &para_before = std::get<ast::ParagraphBreak>(trailing[0]);
    REQUIRE(para_before.blank_lines == 1);

    // Second should be the comment
    REQUIRE(std::holds_alternative<ast::Comment>(trailing[1]));
    const auto &comment = std::get<ast::Comment>(trailing[1]);
    REQUIRE(comment.text.contains("test"));

    // Third should be a paragraph break (blank line after comment)
    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(trailing[2]));
    const auto &para_after = std::get<ast::ParagraphBreak>(trailing[2]);
    REQUIRE(para_after.blank_lines == 1);

    // 'two' should have no leading trivia
    // Convenience method handles the nullptr case automatically!
    REQUIRE(two.getLeading().empty());
}

TEST_CASE("Generic with inline comment captures paragraph breaks correctly",
          "[design_units][comments]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity ExampleEntity is
            generic (
                one : integer := 1; -- inline

                -- test

                two : integer := 2
            );
        end ExampleEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 2);

    const auto &one = entity->generic_clause.generics[0];
    const auto &two = entity->generic_clause.generics[1];

    REQUIRE(one.trivia != nullptr);

    // Check inline comment
    const auto inline_com = one.getInlineComment();
    REQUIRE(inline_com.has_value());
    REQUIRE(inline_com->contains("inline"));

    // Check trailing
    const auto trailing = one.getTrailing();
    REQUIRE(trailing.size() == 3);

    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(trailing[0]));
    REQUIRE(std::get<ast::ParagraphBreak>(trailing[0]).blank_lines == 1);

    REQUIRE(std::holds_alternative<ast::Comment>(trailing[1]));
    REQUIRE(std::get<ast::Comment>(trailing[1]).text.contains("test"));

    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(trailing[2]));
    REQUIRE(std::get<ast::ParagraphBreak>(trailing[2]).blank_lines == 1);

    // 'two' should have no leading trivia
    REQUIRE(two.getLeading().empty());
}

TEST_CASE("Generic captures paragraph breaks after comments with 2 blank lines",
          "[design_units][comments]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity ExampleEntity is
            generic (
                one : integer := 1;


                -- test


                two : integer := 2
            );
        end ExampleEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 2);

    const auto &one = entity->generic_clause.generics[0];
    const auto &two = entity->generic_clause.generics[1];

    REQUIRE(one.trivia != nullptr);

    const auto trailing = one.getTrailing();
    REQUIRE(trailing.size() == 3);

    // First should be a paragraph break (blank line before comment)
    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(trailing[0]));
    REQUIRE(std::get<ast::ParagraphBreak>(trailing[0]).blank_lines == 2);

    // Second should be the comment
    REQUIRE(std::holds_alternative<ast::Comment>(trailing[1]));
    REQUIRE(std::get<ast::Comment>(trailing[1]).text.contains("test"));

    // Third should be a paragraph break (blank line after comment)
    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(trailing[2]));
    REQUIRE(std::get<ast::ParagraphBreak>(trailing[2]).blank_lines == 2);

    // 'two' should have no leading trivia
    REQUIRE(two.getLeading().empty());
}
