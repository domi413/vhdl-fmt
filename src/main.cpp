#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/nodes/design_file.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/translator.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "builder/visitor.hpp"
#include "cli/argument_parser.hpp"
#include "cli/config_reader.hpp"
#include "emit/debug_printer.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <span>
#include <stdexcept>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        const cli::ArgumentParser argparser{
            std::span<char *>{ argv, static_cast<std::size_t>(argc) }
        };

        const cli::ConfigReader config_reader{ argparser.getConfigPath() };

        // Call the formatter and pass the flag status & config object
        // formatter{ argparser.getFlags(), config_reader.readConfigFile() };

        std::ifstream in(argparser.getInputPath());
        if (!in) {
            throw std::runtime_error("Failed to open input file: "
                                     + argparser.getInputPath().string());
        }

        // Note that these pipelines should be abstracted away into separate classes
        // -- CST construction --
        antlr4::ANTLRInputStream input(in);
        vhdlLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        tokens.fill();

        vhdlParser parser(&tokens);
        auto *tree = parser.design_file();

        // -- AST construction --
        ast::DesignFile root;
        builder::TriviaBinder trivia(tokens);
        builder::Translator translator(trivia, tokens);
        translator.setUnitsDestination(root.units);
        builder::Visitor visitor(translator);
        builder::adapter::AntlrVoidAdapter adapter(visitor);
        tree->accept(&adapter);

        // std::cout << tree->toStringTree(&parser, true) << '\n';

        // -- AST printing for debugging --
        emit::DebugPrinter printer(std::cout);
        printer.visit(root);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
