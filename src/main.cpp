#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/nodes/design_file.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/translator.hpp"
#include "builder/visitor.hpp"
#include "cli/argument_parser.hpp"
#include "cli/config_reader.hpp"
#include "emit/debug_printer.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>

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

        /* NOTE: The following code is from the previous main function

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
           builder::Assembler builder(root.units);

           builder::Translator translator(builder, tokens);

           builder::Visitor visitor(translator);
           visitor.walk(tree);

           std::cout << tree->toStringTree(&parser, true) << "\n\n";

           emit::DebugPrinter printer;
           root.accept(printer);
        */

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
