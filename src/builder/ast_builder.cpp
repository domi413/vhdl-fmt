#include "builder/ast_builder.hpp"

#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "antlr4-runtime/atn/PredictionMode.h"
#include "ast/nodes/design_file.hpp"
#include "builder/translator.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "common/logger.hpp"
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
    auto &logger = common::Logger::instance();

    // CST construction
    antlr4::ANTLRInputStream antlr_input(input);
    vhdlLexer lexer(&antlr_input);

    lexer.removeErrorListeners();

    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    vhdlParser parser(&tokens);

    parser.removeErrorListeners();

    // Performance optimization: Use SLL prediction mode
    // NOTE: SLL might fail for VHDL in certain cases, so we may use a slower
    // but more stable optimization at a certain point.
    parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(
      antlr4::atn::PredictionMode::SLL);

    vhdlParser::Design_fileContext *tree = nullptr;

    try {
        tree = parser.design_file();
    } catch (...) {
        logger.error("SLL failed, retry with LL_EXACT_AMBIG_DETECTION prediction mode");
        tokens.seek(0);
        parser.reset();
        parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(
          antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);
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
