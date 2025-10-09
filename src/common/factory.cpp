#include "config.hpp"
#include "factory.hpp"

#include <filesystem>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

namespace common {

auto ConfigFactory::create(const std::string &working_dir) -> Config
{
    Config::Builder builder;

    // TODO(anyone): Global config, like (.config/...) could be put here

    // Project specific config
    if (auto project_path = findConfig(working_dir)) {
        builder.merge(loadConfigFile(*project_path));
    }

    // TODO(anyone): CLI args could be put here for final overrides

    return builder.build();
}

auto ConfigFactory::findConfig(const std::string &start_dir) -> std::optional<std::string>
{
    fs::path dir = start_dir;
    while (!dir.empty()) {
        fs::path candidate = dir / ".vhdlfmt.yml"; // TODO(anyone): Use constant
        if (fs::exists(candidate)) {
            return candidate.string();
        }
        if (dir == dir.root_path()) {
            break;
        }
        dir = dir.parent_path();
    }
    return std::nullopt;
}

auto ConfigFactory::loadConfigFile(const std::string &path) -> Config::Builder
{
    Config::Builder builder;
    try {
        YAML::Node node = YAML::LoadFile(path);
        if (node["indent_width"]) {
            builder.setIndentWidth(node["indent_width"].as<int>());
        }
        if (node["use_tabs"]) {
            builder.setUseTabs(node["use_tabs"].as<bool>());
        }
        if (node["line_ending"]) {
            builder.setLineEnding(node["line_ending"].as<std::string>());
        }
    } catch (const YAML::Exception &e) {
        std::cerr << "Error parsing config file '" << path << "': " << e.what() << "\n";
    }
    return builder;
}

} // namespace common
