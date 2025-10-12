#ifndef BUILDER_TRIVIA_BINDER_HPP
#define BUILDER_TRIVIA_BINDER_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "builder/trivia/comment_sink.hpp"
#include "builder/trivia/newline_sink.hpp"
#include "vhdlLexer.h"

#include <cstddef>

namespace builder {

/// @brief Builds ordered trivia streams (comments + newlines) for AST nodes.
class TriviaBinder
{
  public:
    explicit TriviaBinder(antlr4::CommonTokenStream &ts) : tokens_(ts) {}

    void bind(ast::Node &node, const antlr4::ParserRuleContext *ctx);

  private:
    struct StopInfo
    {
        std::size_t idx;
        int line;
    };

    antlr4::CommonTokenStream &tokens_;
    CommentSink comments_;
    NewlineSink newlines_;

    static inline bool isComment(const antlr4::Token *t)
    {
        return t && t->getChannel() == vhdlLexer::COMMENTS;
    }

    static inline bool isNewline(const antlr4::Token *t)
    {
        return t && t->getChannel() == vhdlLexer::NEWLINES;
    }

    static std::size_t countLineBreaks(const std::string &s)
    {
        std::size_t n = 0;
        for (char c : s)
            if (c == '\n')
                ++n;
        return n ? n : 1;
    }

    void collectLeading(ast::Node::NodeComments &dst, std::size_t start_idx);
    void collectTrailing(ast::Node::NodeComments &dst, StopInfo stop);
};

} // namespace builder

#endif /* BUILDER_TRIVIA_BINDER_HPP */
