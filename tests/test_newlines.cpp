#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/translator.hpp"
#include "builder/visitor.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <memory>
#include <optional>
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
    builder::Assembler assembler(design->units);
    builder::Translator translator(assembler, tokens);
    builder::Visitor visitor(translator);
    builder::adapter::AntlrVoidAdapter adapter(visitor);
    tree->accept(&adapter);
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
        } else if (const auto *nl = std::get_if<ast::Newlines>(&t)) {
            ++c.newlines_items;
            c.newline_breaks += nl->breaks;
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
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);

    const auto &lead = entity->tryGetTrivia().value_or(ast::Node::NodeTrivia{}).leading;

    // Expect: Comment("A"), Newlines(1), Comment("B")
    REQUIRE(std::holds_alternative<ast::Comments>(lead[0]));
    REQUIRE(std::holds_alternative<ast::Newlines>(lead[1]));
    REQUIRE(std::holds_alternative<ast::Comments>(lead[2]));

    if (std::holds_alternative<ast::Newlines>(lead[1])) {
        const auto &nl = std::get<ast::Newlines>(lead[1]);
        REQUIRE(nl.breaks == 2); // 2 breaks = 1 blank line
    }
}
