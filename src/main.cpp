#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/nodes/design_file.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/translator.hpp"
#include "builder/visitor.hpp"
#include "emit/debug_printer.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>

auto main(int argc, char *argv[]) -> int
{
    const std::span args(argv, static_cast<std::size_t>(argc));

    if (args.size() < 2) {
        std::cerr << "Usage: vhdl_formatter <file.vhdl>\n";
        return 1;
    }

    const std::filesystem::path path{ args[1] };
    const std::ifstream in(path);
    if (!in) {
        std::cerr << "Could not open input file: " << path << '\n';
        return 1;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    antlr4::ANTLRInputStream input(buffer.str());
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    vhdlParser parser(&tokens);
    auto *tree = parser.design_file();

    //--- AST construction pipeline ---
    ast::DesignFile root;
    builder::Assembler assembler(root.units);
    builder::Translator translator(assembler, tokens);
    builder::Visitor visitor(translator);
    builder::adapter::AntlrVoidAdapter adapter(visitor);

    tree->accept(&adapter);

    std::cout << tree->toStringTree(&parser, true) << "\n\n";

    emit::DebugPrinter printer;
    root.accept(printer);

    return 0;
}
