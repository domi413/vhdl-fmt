#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "absl/strings/match.h"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/translator.hpp"
#include "builder/visitor.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>
#include <string>

namespace {

auto loadVhdl(const std::string &filename) -> std::string
{
    const std::ifstream file(filename);
    REQUIRE(file.is_open());
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace

TEST_CASE("Attach VHDL comments to AST nodes", "[integration][comments]")
{
    // --- Input VHDL with rich comments ---
    const std::string vhdl_code{ loadVhdl(std::string(TEST_DATA_DIR) + "/comments.vhdl") };

    // --- Setup ANTLR ---
    antlr4::ANTLRInputStream input(vhdl_code);
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    vhdlParser parser(&tokens);

    auto *tree = parser.design_file();

    // --- Build AST ---
    ast::DesignFile design;
    builder::Assembler assembler(design.units);
    builder::Translator translator(assembler, tokens);
    builder::Visitor visitor(translator);
    builder::adapter::AntlrVoidAdapter adapter(visitor);
    tree->accept(&adapter);

    // --- Verify AST and comments ---
    REQUIRE(design.units.size() == 1);
    auto *entity = dynamic_cast<ast::Entity *>(design.units[0].get());
    REQUIRE(entity != nullptr);

    // Leading comment block above the entity
    REQUIRE(entity->tryGetComments().has_value());
    const auto &comments = entity->tryGetComments()->leading;
    REQUIRE_FALSE(comments.empty());
    REQUIRE(absl::StrContains(comments.front().text, "Entity declaration"));

    // Generic parameters: check CONST_V for leading and inline
    REQUIRE(entity->generics.size() > 0);
    const auto &first_generic = *entity->generics.front();
    REQUIRE(first_generic.tryGetComments().has_value());

    const auto &g_comments = first_generic.tryGetComments().value();
    REQUIRE_FALSE(g_comments.leading.empty());
    REQUIRE(absl::StrContains(g_comments.leading.front().text, "Configuration constants"));

    // Check inline trailing comment (e.g. "-- Example constant")
    REQUIRE_FALSE(g_comments.trailing.empty());
    REQUIRE(g_comments.trailing.front().is_inline);
    REQUIRE(absl::StrContains(g_comments.trailing.front().text, "Example constant"));
}
