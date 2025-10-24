#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/translator.hpp"
#include "builder/trivia/trivia_binder.hpp"
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
    builder::TriviaBinder trivia(tokens);
    builder::Translator translator(trivia, tokens);
    translator.setUnitsDestination(design->units);
    builder::Visitor visitor(translator);
    builder::adapter::AntlrVoidAdapter adapter(visitor);

    tree->accept(&adapter);
    return design;
}

// Extract only comment texts from LEADING (vector<Trivia>)
auto leadingComments(const std::vector<ast::Trivia> &tv) -> std::vector<std::string_view>
{
    return tv
         | std::views::filter(
             [](const ast::Trivia &t) -> bool { return std::holds_alternative<ast::Comments>(t); })
         | std::views::transform([](const ast::Trivia &t) -> std::string_view {
               return std::get<ast::Comments>(t).text;
           })
         | std::ranges::to<std::vector<std::string_view>>();
}

// Extract texts from TRAILING (vector<CommentTrivia>)
auto trailingComments(const std::vector<ast::Comments> &tv) -> std::vector<std::string_view>
{
    std::vector<std::string_view> out;
    out.reserve(tv.size());
    for (const auto &c : tv) {
        out.push_back(c.text);
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
    auto *entity = std::get_if<ast::Entity>(design->units.data());
    REQUIRE(entity != nullptr);

    const auto &leading_trivia = entity->tryGetTrivia().value_or(ast::NodeTrivia{}).leading;
    const auto texts = leadingComments(leading_trivia);

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
    const auto &c = g.tryGetTrivia().value_or(ast::NodeTrivia{});

    const auto lead = leadingComments(c.leading);
    const auto trail = trailingComments(c.trailing);

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
        const auto &cm = clk.tryGetTrivia().value_or(ast::NodeTrivia{});
        const auto lead = leadingComments(cm.leading);
        const auto trail = trailingComments(cm.trailing);
        REQUIRE_FALSE(lead.empty());
        REQUIRE(lead.front().contains("Clock input"));
        REQUIRE_FALSE(trail.empty());
        REQUIRE(trail.front().contains("inline clock"));
    }
    {
        const auto &rst = entity->port_clause.ports[1];
        const auto &cm = rst.tryGetTrivia().value_or(ast::NodeTrivia{});
        const auto lead = leadingComments(cm.leading);
        const auto trail = trailingComments(cm.trailing);
        REQUIRE_FALSE(lead.empty());
        REQUIRE(lead.front().contains("Reset input"));
        REQUIRE_FALSE(trail.empty());
        REQUIRE(trail.front().contains("inline reset"));
    }
}
