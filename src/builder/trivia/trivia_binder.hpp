#ifndef BUILDER_TRIVIA_TRIVIA_BINDER_HPP
#define BUILDER_TRIVIA_TRIVIA_BINDER_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "ast/node.hpp"

#include <cstddef>

namespace builder {

/// @brief Builds ordered trivia streams (comments + newlines) for AST nodes.
class TriviaBinder final
{
  public:
    explicit TriviaBinder(antlr4::CommonTokenStream &ts) noexcept : tokens_(ts) {}

    ~TriviaBinder() = default;

    TriviaBinder(const TriviaBinder &) = delete;
    auto operator=(const TriviaBinder &) -> TriviaBinder & = delete;
    TriviaBinder(TriviaBinder &&) = delete;
    auto operator=(TriviaBinder &&) -> TriviaBinder & = delete;

    /// @brief Binds collected trivia to the specified AST node.
    void bind(ast::NodeBase &node, const antlr4::ParserRuleContext *ctx);

  private:
    antlr4::CommonTokenStream &tokens_;
    std::unordered_set<std::size_t> used_; ///< set of token indices already added as trivia

    void collect(std::vector<ast::Trivia> &dst, const std::vector<antlr4::Token *> &tokens);
    auto collectInline(std::optional<ast::Comment> &dst, std::size_t index);

    [[nodiscard]]
    auto findLastDefaultOnLine(std::size_t start_index) const -> std::size_t;
};

} // namespace builder

#endif /* BUILDER_TRIVIA_TRIVIA_BINDER_HPP */
