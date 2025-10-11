#include "absl/strings/match.h"
#include "cli/config_reader.hpp"
#include "common/config.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>

namespace {

// RAII class for scope exit cleanup
template<typename F>
class ScopeExit
{
  public:
    ScopeExit(const ScopeExit &) = delete;
    ScopeExit(ScopeExit &&) = delete;
    auto operator=(const ScopeExit &) -> ScopeExit & = delete;
    auto operator=(ScopeExit &&) -> ScopeExit & = delete;
    explicit ScopeExit(F func) : func_(std::move(func)) {}
    ~ScopeExit() { func_(); }

  private:
    F func_;
};

template<typename F>
auto makeScopeExit(F func) -> ScopeExit<F>
{
    return ScopeExit<F>(std::move(func));
}

constexpr auto getConfigPath(const std::string_view filename) -> std::filesystem::path
{
    return std::filesystem::path{ TEST_DATA_DIR } / "config_file" / filename;
}

} // namespace

SCENARIO("ConfigReader handles configuration file reading")
{
    GIVEN("A valid complete configuration file")
    {
        const auto config_path = getConfigPath("valid_complete.yaml");
        cli::ConfigReader config_reader{ config_path };

        WHEN("the config file is read")
        {
            const auto result = config_reader.readConfigFile();

            THEN("it succeeds and contains the expected values")
            {
                REQUIRE(result.has_value());
                const auto &config = result.value();

                REQUIRE(config.line_config.line_length == 120);
                REQUIRE(config.line_config.indent_size == 2);
                REQUIRE(config.indent_style == common::IndentationStyle::SPACES);
                REQUIRE(config.eol_format == common::EndOfLine::LF);
                REQUIRE(config.port_map.align_signals);
                REQUIRE(config.declarations.align_colons);
                REQUIRE(config.declarations.align_types);
                REQUIRE(config.declarations.align_initialization);
                REQUIRE(config.casing.keywords == common::CaseStyle::LOWER);
                REQUIRE(config.casing.constants == common::CaseStyle::UPPER);
                REQUIRE(config.casing.identifiers == common::CaseStyle::LOWER);
            }
        }
    }

    GIVEN("An empty configuration file")
    {
        const auto config_path = getConfigPath("empty.yaml");
        cli::ConfigReader reader{ config_path };

        WHEN("the config file is read")
        {
            const auto result = reader.readConfigFile();

            THEN("it succeeds with all default values")
            {
                REQUIRE(result.has_value());
                const auto &config = result.value();

                REQUIRE(config.line_config.line_length == 100);
                REQUIRE(config.line_config.indent_size == 4);
                REQUIRE(config.indent_style == common::IndentationStyle::SPACES);
                REQUIRE(config.eol_format == common::EndOfLine::AUTO);
                REQUIRE(config.port_map.align_signals);
                REQUIRE(config.declarations.align_colons);
                REQUIRE(config.declarations.align_types);
                REQUIRE(config.declarations.align_initialization);
                REQUIRE(config.casing.keywords == common::CaseStyle::LOWER);
                REQUIRE(config.casing.constants == common::CaseStyle::UPPER);
                REQUIRE(config.casing.identifiers == common::CaseStyle::LOWER);
            }
        }
    }

    GIVEN("A non-existent configuration file")
    {
        const auto config_path = getConfigPath("non_existent_file.yaml");
        cli::ConfigReader reader{ config_path };

        WHEN("the config file is read")
        {
            const auto result = reader.readConfigFile();

            THEN("it fails with specific error message")
            {
                REQUIRE_FALSE(result.has_value());
                const auto &error = result.error();
                REQUIRE(error.message == "Config file does not exist at the defined location.");
            }
        }
    }

    GIVEN("A malformed YAML configuration file")
    {
        const auto config_path = getConfigPath("malformed.yaml");
        cli::ConfigReader reader{ config_path };

        WHEN("the config file is read")
        {
            const auto result = reader.readConfigFile();

            THEN("it fails with parsing error")
            {
                REQUIRE_FALSE(result.has_value());
                const auto &error = result.error();
                REQUIRE(absl::StrContains(error.message, "Error reading config file"));
            }
        }
    }

    GIVEN("No config file path provided and no default config file exists")
    {
        // Temporarily change to a directory without vhdl-fmt.yaml
        const auto original_path = std::filesystem::current_path();
        const auto temp_dir = std::filesystem::temp_directory_path() / "test_no_config";
        std::filesystem::create_directories(temp_dir);
        std::filesystem::current_path(temp_dir);

        auto cleanup_guard = makeScopeExit([&] {
            std::filesystem::current_path(original_path);
            std::filesystem::remove_all(temp_dir);
        });

        cli::ConfigReader reader{ std::nullopt };

        WHEN("the config file is read")
        {
            const auto result = reader.readConfigFile();

            THEN("it succeeds with all default values")
            {
                REQUIRE(result.has_value());
                const auto &config = result.value();

                REQUIRE(config.line_config.line_length == 100);
                REQUIRE(config.line_config.indent_size == 4);
                REQUIRE(config.indent_style == common::IndentationStyle::SPACES);
                REQUIRE(config.eol_format == common::EndOfLine::AUTO);
            }
        }
    }

    GIVEN("An invalid configuration parameter")
    {
        auto [description, content, expected_error]
          = GENERATE(table<std::string, std::string, std::string>({
            { "line length too small",
             "line_length: 9\n",                                         "Line length must be between 10 and 200"          },
            { "line length too large",
             "line_length: 500\n",                                       "Line length must be between 10 and 200"          },
            { "indent size too small",
             "indentation:\n  size: 0\n",                                "Indent size must be between 1 and 16"            },
            { "indent size too large",
             "indentation:\n  size: 20\n",                               "Indent size must be between 1 and 16"            },
            { "invalid indent style",
             "indentation:\n  style: \"invalid_style\"\n",               "Invalid indentation style config: invalid_style" },
            { "invalid end of line",
             "end_of_line: \"invalid_eol\"\n",                           "Invalid end of line style config: invalid_eol"   },
            { "invalid casing style",
             "formatting:\n  casing:\n    keywords: \"invalid_case\"\n", "Invalid casing config: invalid_case"             }
        }));

        WHEN(description)
        {
            const auto temp_path
              = std::filesystem::temp_directory_path() / "test_invalid_config.yaml";
            {
                std::ofstream temp_file{ temp_path };
                temp_file << content;
            }

            auto cleanup_guard = makeScopeExit([&] { std::filesystem::remove(temp_path); });

            cli::ConfigReader reader{ temp_path };
            const auto result = reader.readConfigFile();

            THEN("it fails with specific error message")
            {
                INFO("Description: " << description);
                INFO("Content: " << content);
                if (result.has_value()) {
                    INFO("Unexpectedly succeeded - config was accepted");
                    const auto &config = result.value();
                    INFO("Line length: " << static_cast<int>(config.line_config.line_length));
                    INFO("Indent size: " << static_cast<int>(config.line_config.indent_size));
                }
                REQUIRE_FALSE(result.has_value());
                const auto &error = result.error();
                INFO("Expected: " << expected_error);
                INFO("Actual: " << error.message);
                REQUIRE(absl::StrContains(error.message, expected_error));
            }
        }
    }

    GIVEN("Edge case: boundary values for line length and indent size")
    {
        auto [description, content, expected_value, expected_field]
          = GENERATE(table<std::string, std::string, std::uint8_t, std::string>({
            { "line length minimum (10)",  "line_length: 10\n",          10,  "line_length" },
            { "line length maximum (200)", "line_length: 200\n",         200, "line_length" },
            { "indent size minimum (1)",   "indentation:\n  size: 1\n",  1,   "indent_size" },
            { "indent size maximum (16)",  "indentation:\n  size: 16\n", 16,  "indent_size" }
        }));

        WHEN(description)
        {
            const auto temp_path = std::filesystem::temp_directory_path()
                                 / std::format("test_{}.yaml", std::to_string(expected_value));
            {
                std::ofstream temp_file{ temp_path };
                temp_file << content;
            }

            auto cleanup_guard = makeScopeExit([&] { std::filesystem::remove(temp_path); });

            cli::ConfigReader reader{ temp_path };
            const auto result = reader.readConfigFile();

            THEN("it succeeds with the expected value")
            {
                REQUIRE(result.has_value());
                const auto &config = result.value();

                if (expected_field == "line_length") {
                    REQUIRE(config.line_config.line_length == expected_value);
                } else if (expected_field == "indent_size") {
                    REQUIRE(config.line_config.indent_size == expected_value);
                }
            }
        }
    }
}
