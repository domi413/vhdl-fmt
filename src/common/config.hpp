#ifndef CONFIG_CONFIG_HPP
#define CONFIG_CONFIG_HPP

#include <optional>
#include <string>

namespace common {

class Config
{
    int indent_width;
    bool use_tabs;
    std::string line_ending;

    // Private constructor, only `Config::Builder` can use
    Config(int indent_width, bool use_tabs, std::string line_ending) :
      indent_width(indent_width),
      use_tabs(use_tabs),
      line_ending(std::move(line_ending))
    {
    }

  public:
    class Builder;

    // Accessors
    [[nodiscard]] auto indentWidth() const noexcept -> int { return indent_width; }
    [[nodiscard]] auto useTabs() const noexcept -> bool { return use_tabs; }
    [[nodiscard]] auto lineEnding() const noexcept -> const std::string & { return line_ending; }
};

class Config::Builder
{
    std::optional<int> indent_width;
    std::optional<bool> use_tabs;
    std::optional<std::string> line_ending;

  public:
    auto setIndentWidth(int value) noexcept -> Builder &
    {
        indent_width = value;
        return *this;
    }
    auto setUseTabs(bool value) noexcept -> Builder &
    {
        use_tabs = value;
        return *this;
    }
    auto setLineEnding(const std::string &value) noexcept -> Builder &
    {
        line_ending = value;
        return *this;
    }

    [[nodiscard]] auto build() const -> Config
    {
        // Use defaults if not set
        return Config{ indent_width.value_or(4),
                       use_tabs.value_or(false),
                       line_ending.value_or("lf") };
    }

    void merge(const Builder &other);
};

} // namespace common

#endif /* CONFIG_CONFIG_HPP */
