#ifndef BUILDER_TRIVIA_NEWLINE_SINK_HPP
#define BUILDER_TRIVIA_NEWLINE_SINK_HPP

#include "ast/node.hpp"

#include <cstddef>
#include <utility>

namespace builder {

/// @brief Appends newline trivia (aggregated line breaks) to a node.
class NewlineSink
{
  public:
    void push(ast::Node::NodeComments &dst, bool to_leading, std::size_t breaks)
    {
        if (breaks == 0)
            return;

        ast::Trivia tv;
        tv.kind = ast::Trivia::Kind::Newlines;
        tv.breaks = breaks;

        auto &vec = to_leading ? dst.leading : dst.trailing;
        vec.push_back(std::move(tv));
    }
};

} // namespace builder

#endif /* BUILDER_TRIVIA_NEWLINE_SINK_HPP */
