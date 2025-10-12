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
#include <string_view>

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

} // namespace

// -----------------------------------------------------------------------------
// Test 1: Entity-level leading comments
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

    const auto &comments = entity->tryGetComments().value_or(ast::Node::NodeComments{}).leading;
    REQUIRE(comments.size() == 2);
    REQUIRE(std::string_view{ comments.front().text }.contains("License text"));
    REQUIRE(std::string_view{ comments.back().text }.contains("Entity declaration"));
}

// -----------------------------------------------------------------------------
// Test 2: Generic-level leading and inline comments
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

    REQUIRE_FALSE(c.leading.empty());
    REQUIRE(std::string_view{ c.leading.front().text }.contains("Leading for CONST_V"));

    REQUIRE_FALSE(c.trailing.empty());
    REQUIRE(std::string_view{ c.trailing.front().text }.contains("Inline for CONST_V"));
}

// -----------------------------------------------------------------------------
// Test 3: Port-level leading and inline comments
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

    const auto &clk = *entity->ports[0];
    const auto &clk_c = clk.tryGetComments().value_or(ast::Node::NodeComments{});
    REQUIRE_FALSE(clk_c.leading.empty());
    REQUIRE(std::string_view{ clk_c.leading.front().text }.contains("Clock input"));
    REQUIRE_FALSE(clk_c.trailing.empty());
    REQUIRE(std::string_view{ clk_c.trailing.front().text }.contains("inline clock"));

    const auto &rst = *entity->ports[1];
    const auto &rst_c = rst.tryGetComments().value_or(ast::Node::NodeComments{});
    REQUIRE_FALSE(rst_c.leading.empty());
    REQUIRE(std::string_view{ rst_c.leading.front().text }.contains("Reset input"));
    REQUIRE_FALSE(rst_c.trailing.empty());
    REQUIRE(std::string_view{ rst_c.trailing.front().text }.contains("inline reset"));
}

// -----------------------------------------------------------------------------
// Test 4: Mixed leading comment block and newlines
// -----------------------------------------------------------------------------
TEST_CASE("Consecutive comment block with newlines is preserved", "[comments][multiline]")
{
    const std::string vhdl = R"(
        -- Header line 1
        --
        -- Header line 3
        entity Blocky is end Blocky;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *entity = dynamic_cast<ast::Entity *>(design->units[0].get());
    REQUIRE(entity != nullptr);

    const auto &comments = entity->tryGetComments().value_or(ast::Node::NodeComments{}).leading;
    REQUIRE(comments.size() == 3);
    REQUIRE(std::string_view{ comments.front().text }.contains("Header line 1"));
    REQUIRE(std::string_view{ comments.back().text }.contains("Header line 3"));
}
