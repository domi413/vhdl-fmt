#include "Config.hpp"

#include <stdexcept>

namespace vhdl_fmt {

auto Config::validate() const -> void
{
    if (line_config.line_length < MIN_LINE_LENGTH || line_config.line_length > MAX_LINE_LENGTH) {
        const std::string msg = "Line length must be between "
                              + std::to_string(MIN_LINE_LENGTH)
                              + " and "
                              + std::to_string(MAX_LINE_LENGTH)
                              + ".";

        throw std::invalid_argument(msg);
    }

    if (line_config.indent_size < MIN_INDENT_SIZE || line_config.indent_size > MAX_INDENT_SIZE) {
        const std::string msg = "Indent size must be between "
                              + std::to_string(MIN_INDENT_SIZE)
                              + " and "
                              + std::to_string(MAX_INDENT_SIZE)
                              + ".";

        throw std::invalid_argument(msg);
    }
}
} // namespace vhdl_fmt
