#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/node.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/translator.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace {

auto buildAstFromSource(const std::string &vhdl_code) -> std::unique_ptr<ast::DesignFile>
{
    antlr4::ANTLRInputStream input(vhdl_code);
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    vhdlParser parser(&tokens);

    auto *tree = parser.design_file();

    auto design = std::make_unique<ast::DesignFile>();
    builder::TriviaBinder trivia(tokens);
    builder::Translator translator(trivia, tokens);
    translator.buildDesignFile(*design, tree);

    return design;
}

struct Counts
{
    std::size_t comments{ 0 };
    std::size_t newlines_items{ 0 }; // number of Newlines trivia items
    std::size_t newline_breaks{ 0 }; // total breaks across all Newlines items
};

auto tallyTrivia(const std::vector<ast::Trivia> &tv) -> Counts
{
    return std::ranges::fold_left(tv, Counts{}, [](Counts c, const ast::Trivia &t) -> Counts {
        if (std::holds_alternative<ast::Comments>(t)) {
            ++c.comments;
        } else if (const auto *pb = std::get_if<ast::ParagraphBreak>(&t)) {
            ++c.newlines_items;
            // Convert blank_lines back to breaks for compatibility (breaks = blank_lines + 1)
            c.newline_breaks += pb->blank_lines + 1;
        }

        return c;
    });
}

} // namespace

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
