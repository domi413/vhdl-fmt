#include "builder/ast_builder.hpp"

#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "antlr4-runtime/atn/PredictionMode.h"
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

    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    vhdlParser parser(&tokens);

    // NOTE: SLL would be a significant performance improvement, though may
    //       result in incorrect parsing for VHDL. -> We should try to
    //       "optimize" parsing when everything else seems to be working.
    parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->setPredictionMode(
      antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);

    vhdlParser::Design_fileContext *tree = nullptr;

    tree = parser.design_file();

    // AST construction
    ast::DesignFile root{};
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
