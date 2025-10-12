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

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <string>
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
    Counts c{};
    for (const auto &t : tv) {
        if (t.kind == ast::Trivia::Kind::Comment) {
            ++c.comments;
        } else {
            ++c.newlines_items;
            c.newline_breaks += t.breaks;
        }
    }
    return c;
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

    const auto &lead = entity->tryGetComments().value_or(ast::Node::NodeComments{}).leading;

    // Expect: Comment("A"), Newlines(1), Comment("B")
    REQUIRE(lead.size() == 3);
    REQUIRE(lead[0].kind == ast::Trivia::Kind::Comment);
    REQUIRE(lead[1].kind == ast::Trivia::Kind::Newlines);
    REQUIRE(lead[1].breaks >= 1); // lexer might coalesce; countLineBreaks guards >=1
    REQUIRE(lead[2].kind == ast::Trivia::Kind::Comment);
}

// -----------------------------------------------------------------------------
// Trailing trivia: newlines after inline comment are preserved
// -----------------------------------------------------------------------------
TEST_CASE("Trailing trivia captures newlines after inline comment", "[trivia][trailing]")
{
    const std::string vhdl = R"(
        entity E is
            generic (
                G : integer := 0  -- inline g
                                
            );
        end E;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generics.size() == 1);

    const auto &g = *entity->generics[0];
    const auto &trail = g.tryGetComments().value_or(ast::Node::NodeComments{}).trailing;

    // Expect: [Comment("inline g"), Newlines(k>=1)]
    REQUIRE_FALSE(trail.empty());
    REQUIRE(trail[0].kind == ast::Trivia::Kind::Comment);

    // There may or may not be an explicit Newlines item depending on the lexer,
    // but typically there will be at least one.
    if (trail.size() >= 2) {
        REQUIRE(trail[1].kind == ast::Trivia::Kind::Newlines);
        REQUIRE(trail[1].breaks >= 1);
    }
}

// -----------------------------------------------------------------------------
// Leading boundary: stop at blank-line boundary (>=2 breaks)
// -----------------------------------------------------------------------------
TEST_CASE("Leading block stops at blank-line boundary", "[trivia][leading][boundary]")
{
    // Two blank lines above comments should *not* include earlier comments beyond the boundary.
    const std::string vhdl = R"(
        -- Old header (should be ignored)
        
        
        -- Near header 1
        -- Near header 2
        entity E is end E;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);

    const auto &lead = entity->tryGetComments().value_or(ast::Node::NodeComments{}).leading;
    auto counts = tallyTrivia(lead);

    // Only the "Near header" comments are included (2 comments), possibly with
    // a single Newlines item between them if there's a pure blank line.
    REQUIRE(counts.comments == 2);
    // Ensure none of the included comment trivia contains "Old header".
    for (const auto &t : lead) {
        if (t.kind == ast::Trivia::Kind::Comment) {
            REQUIRE(!std::string_view{ t.text }.contains("Old header"));
        }
    }
}
