#include "ast/node.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <variant>

using test_utils::buildAstFromSource;

// -----------------------------------------------------------------------------
// Leading trivia: pure blank lines between comments are preserved as Newlines
// -----------------------------------------------------------------------------
TEST_CASE("Leading trivia preserves pure blank lines between comments", "[trivia][leading]")
{
    // One blank *source* line between two leading comments.
    const std::string vhdl = R"(
        -- A

        -- B
        entity E is end E;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = std::get_if<ast::Entity>(design->units.data());
    REQUIRE(entity != nullptr);

    const auto &lead = entity->tryGetTrivia().value_or(ast::NodeTrivia{}).leading;

    // Expect: Comment("A"), ParagraphBreak(1 blank line), Comment("B")
    REQUIRE(std::holds_alternative<ast::Comments>(lead[0]));
    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(lead[1]));
    REQUIRE(std::holds_alternative<ast::Comments>(lead[2]));

    if (std::holds_alternative<ast::ParagraphBreak>(lead[1])) {
        const auto &pb = std::get<ast::ParagraphBreak>(lead[1]);
        REQUIRE(pb.blank_lines == 1); // 1 blank line
    }
}
