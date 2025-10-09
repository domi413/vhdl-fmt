#ifndef COMMON_FACTORY_HPP
#define COMMON_FACTORY_HPP

#include "config.hpp"

#include <optional>
#include <string>

namespace common {

class ConfigFactory
{
  public:
    static auto create(const std::string &working_dir) -> Config;

  private:
    static auto findConfig(const std::string &start_dir) -> std::optional<std::string>;
    static auto loadConfigFile(const std::string &path) -> Config::Builder;
};
} // namespace common

#endif /* COMMON_FACTORY_HPP */
