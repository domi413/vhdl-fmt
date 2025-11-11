#include "ast/nodes/design_file.hpp"
#include "builder/ast_builder.hpp"
#include "cli/argument_parser.hpp"
#include "cli/config_reader.hpp"
#include "common/config.hpp"
#include "emit/pretty_printer.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>

auto main(int argc, char *argv[]) -> int
{
    try {
        const cli::ArgumentParser argparser{
            std::span<const char *const>{ argv, static_cast<std::size_t>(argc) }
        };

        cli::ConfigReader config_reader{ argparser.getConfigPath() };
        const auto config_result = config_reader.readConfigFile();
        const auto &config = config_result.value();

        // Build AST from input file
        const ast::DesignFile root = builder::buildFromFile(argparser.getInputPath());

        // Pretty print the AST
        const emit::PrettyPrinter printer{};
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
