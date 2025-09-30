#include <span>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        vhdl_fmt::ConfigManager config{ std::span<char *>(argv, static_cast<std::size_t>(argc)) };

    } catch () {
    }

    return 0;
}
