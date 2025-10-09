#include "config.hpp"

namespace common {

void Config::Builder::merge(const Builder &other)
{
    indent_width = (other.indent_width) ? other.indent_width : indent_width;
    line_ending = (other.line_ending) ? other.line_ending : line_ending;
    use_tabs = (other.use_tabs) ? other.use_tabs : use_tabs;
}

} // namespace common