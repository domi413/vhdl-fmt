#include "ast/nodes/design_file.hpp"
#include "builder/ast_builder.hpp"
#include "cli/argument_parser.hpp"
#include "cli/config_reader.hpp"
// #include "emit/debug_printer.hpp"
#include "emit/pretty_printer.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>
#include <stdexcept>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        const cli::ArgumentParser argparser{
            std::span<const char *const>{ argv, static_cast<std::size_t>(argc) }
        };

        cli::ConfigReader config_reader{ argparser.getConfigPath() };
        auto config_result = config_reader.readConfigFile();
        if (!config_result.has_value()) {
            throw std::runtime_error("Failed to read config file");
        }
        const common::Config config = config_result.value();

        // Build AST from input file
        const ast::DesignFile root = builder::buildFromFile(argparser.getInputPath());

        // Use DebugPrinter to print AST
        // emit::DebugPrinter debug_printer(std::cout);
        // debug_printer.visit(root);

        // Use pretty printer to print AST
        emit::PrettyPrinter printer;
        const auto doc = printer(root);
        std::cout << doc.render(config);

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
