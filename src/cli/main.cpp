#include "argument_parser.hpp"
#include "config_reader.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        const cli::ArgumentParser argparser{
            std::span<char *>{ argv, static_cast<std::size_t>(argc) }
        };

        const cli::ConfigReader config_reader{ argparser.getConfigPath() };

        // Call the formatter and pass the config object
        // formatter{ config_reader.readConfigFile() };

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
