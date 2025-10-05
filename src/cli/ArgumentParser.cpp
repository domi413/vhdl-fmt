#include "ArgumentParser.hpp"

#include "version.hpp"
#include "version.hpp.in"

#include <argparse/argparse.hpp>

namespace vhdl_fmt {

auto ArgumentParser::getConfigPath() const -> const std::filesystem::path &
{
    return this->config_file_path;
}

auto ArgumentParser::parseArguments(std::span<char *> args) -> void
{
    argparse::ArgumentParser program{ PROJECT_NAME, PROJECT_VERSION };

    program.add_argument("-v", "--version")
      .help("increase output verbosity")
      .default_value(false)
      .implicit_value(true);
}

auto ArgumentParser::showversion() -> void
{
    //
}

} // namespace vhdl_fmt
