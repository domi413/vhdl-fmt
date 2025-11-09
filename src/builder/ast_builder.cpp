#include "builder/ast_builder.hpp"

#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/nodes/design_file.hpp"
#include "builder/translator.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <filesystem>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace builder {

auto buildFromFile(const std::filesystem::path &path) -> ast::DesignFile
{
    std::ifstream file{ path };
    if (!file) {
        throw std::runtime_error("Failed to open input file: " + path.string());
    }
    return buildFromStream(file);
}

auto buildFromStream(std::istream &input) -> ast::DesignFile
{
    // CST construction
    antlr4::ANTLRInputStream antlr_input{ input };
    vhdlLexer lexer{ &antlr_input };
    antlr4::CommonTokenStream tokens{ &lexer };
    tokens.fill();

    vhdlParser parser{ &tokens };
    auto *tree{ parser.design_file() };

    // AST construction
    ast::DesignFile root{};
    TriviaBinder trivia{ tokens };
    Translator translator{ trivia, tokens };
    translator.buildDesignFile(root, tree);

    return root;
}

auto buildFromString(std::string_view vhdl_code) -> ast::DesignFile
{
    std::istringstream stream{ std::string(vhdl_code) };
    return buildFromStream(stream);
}

} // namespace builder
