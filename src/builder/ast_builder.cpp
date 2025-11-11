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
#include <string_view>

namespace builder {

auto buildFromFile(const std::filesystem::path &path) -> ast::DesignFile
{
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open input file: " + path.string());
    }
    return buildFromStream(file);
}

auto buildFromStream(std::istream &input) -> ast::DesignFile
{
    // CST construction
    antlr4::ANTLRInputStream antlr_input(input);
    vhdlLexer lexer(&antlr_input);

    lexer.removeErrorListeners();

    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    vhdlParser parser(&tokens);

    parser.removeErrorListeners();

    // TODO(niekdomi): Get familiar with the different parsers

    // Performance optimization: Use SLL prediction mode first (faster)
    // SLL mode is much faster but may fail on complex grammar constructs
    parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(
      antlr4::atn::PredictionMode::SLL);

    vhdlParser::Design_fileContext *tree = nullptr;

    try {
        tree = parser.design_file();
    } catch (...) {
        // SLL failed, retry with full LL prediction mode
        tokens.seek(0);
        parser.reset();
        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(
          antlr4::atn::PredictionMode::LL);
        tree = parser.design_file();
    }

    // AST construction
    ast::DesignFile root;
    TriviaBinder trivia(tokens);
    Translator translator(trivia, tokens);
    translator.buildDesignFile(root, tree);

    return root;
}

auto buildFromString(std::string_view vhdl_code) -> ast::DesignFile
{
    std::istringstream stream{ std::string(vhdl_code) };
    return buildFromStream(stream);
}

} // namespace builder
