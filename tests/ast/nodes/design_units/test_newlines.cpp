#include "ast/node.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Leading trivia preserves pure blank lines between comments", "[trivia][leading]")
{
    // One blank *source* line between two leading comments.
    constexpr std::string_view VHDL_FILE = R"(
        -- A

        -- B
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->trivia.has_value());

    const auto &trivia = entity->trivia.value();
    const auto &lead = trivia.leading;

    // Expect: Comment("A"), ParagraphBreak(1 blank line), Comment("B")
    REQUIRE(std::holds_alternative<ast::Comments>(lead[0]));
    REQUIRE(std::holds_alternative<ast::ParagraphBreak>(lead[1]));
    REQUIRE(std::holds_alternative<ast::Comments>(lead[2]));

    if (std::holds_alternative<ast::ParagraphBreak>(lead[1])) {
        const auto &pb = std::get<ast::ParagraphBreak>(lead[1]);
        REQUIRE(pb.blank_lines == 1); // 1 blank line
    }
}
