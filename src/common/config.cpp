#include "config.hpp"

namespace common {

//--------------------------------------
// Builder merging logic
//--------------------------------------
void Config::Builder::merge(const Builder &other)
{
    if (other.indent_width != 4) {
        indent_width = other.indent_width;
    }
    if (other.use_tabs != false) {
        use_tabs = other.use_tabs;
    }
    if (other.line_ending != "lf") {
        line_ending = other.line_ending;
    }
}

} // namespace common