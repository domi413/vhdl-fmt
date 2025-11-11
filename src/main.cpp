#include "ast/nodes/design_file.hpp"
#include "builder/ast_builder.hpp"
#include "cli/argument_parser.hpp"
#include "cli/config_reader.hpp"
#include "emit/debug_printer.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        const cli::ArgumentParser argparser{
            std::span<const char *const>{ argv, static_cast<std::size_t>(argc) }
        };

        const cli::ConfigReader config_reader{ argparser.getConfigPath() };

        // Call the formatter and pass the flag status & config object
        // formatter{ argparser.getFlags(), config_reader.readConfigFile() };

        // Build AST from input file
        const ast::DesignFile root = builder::buildFromFile(argparser.getInputPath());

        // Print AST for debugging
        // emit::DebugPrinter printer(std::cout);
        // printer.visit(root);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
