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
#include <ranges>
#include <string>
#include <string_view>
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

auto commentTexts(const std::vector<ast::Trivia> &tv) -> std::vector<std::string_view>
{
    std::vector<std::string_view> out;
    out.reserve(tv.size());

    for (const std::string_view sv :
         tv | std::views::filter([](const ast::Trivia &t) -> bool {
             return std::holds_alternative<ast::CommentTrivia>(t);
         }) | std::views::transform([](const ast::Trivia &t) -> std::string_view {
             return std::get<ast::CommentTrivia>(t).text;
         })) {
        out.push_back(sv);
    }
    return out;
}

} // namespace

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
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);

    const auto &leading_trivia
      = entity->tryGetComments().value_or(ast::Node::NodeComments{}).leading;
    const auto texts = commentTexts(leading_trivia);

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
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generics.size() == 1);

    const auto &g = *entity->generics[0];
    const auto &c = g.tryGetComments().value_or(ast::Node::NodeComments{});

    const auto lead = commentTexts(c.leading);
    const auto trail = commentTexts(c.trailing);

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
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->ports.size() == 2);

    {
        const auto &clk = *entity->ports[0];
        const auto &cm = clk.tryGetComments().value_or(ast::Node::NodeComments{});
        const auto lead = commentTexts(cm.leading);
        const auto trail = commentTexts(cm.trailing);
        REQUIRE_FALSE(lead.empty());
        REQUIRE(lead.front().contains("Clock input"));
        REQUIRE_FALSE(trail.empty());
        REQUIRE(trail.front().contains("inline clock"));
    }
    {
        const auto &rst = *entity->ports[1];
        const auto &cm = rst.tryGetComments().value_or(ast::Node::NodeComments{});
        const auto lead = commentTexts(cm.leading);
        const auto trail = commentTexts(cm.trailing);
        REQUIRE_FALSE(lead.empty());
        REQUIRE(lead.front().contains("Reset input"));
        REQUIRE_FALSE(trail.empty());
        REQUIRE(trail.front().contains("inline reset"));
    }
}
