#ifndef CONFIG_CONFIG_HPP
#define CONFIG_CONFIG_HPP

#include <string>

namespace common {

class Config
{
  public:
    class Builder;

    // Accessors
    [[nodiscard]] auto indentWidth() const noexcept -> int { return indent_width; }
    [[nodiscard]] auto useTabs() const noexcept -> bool { return use_tabs; }
    [[nodiscard]] auto lineEnding() const noexcept -> const std::string & { return line_ending; }

  private:
    // Private constructor, only `Config::Builder` can use
    Config(int indent_width, bool use_tabs, std::string line_ending) :
      indent_width(indent_width),
      use_tabs(use_tabs),
      line_ending(std::move(line_ending))
    {
    }

    int indent_width;
    bool use_tabs;
    std::string line_ending;
};

class Config::Builder
{
    int indent_width = 4; 
    bool use_tabs = false;
    std::string line_ending = "lf";

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
    auto setLineEnding(std::string value) noexcept -> Builder &
    {
        line_ending = std::move(value);
        return *this;
    }

    [[nodiscard]] auto build() const -> Config { return { indent_width, use_tabs, line_ending }; }

    void merge(const Builder &other);
};

} // namespace common

#endif /* CONFIG_CONFIG_HPP */
