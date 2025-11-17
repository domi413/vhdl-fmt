#include "builder/trivia/trivia_binder.hpp"

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "builder/trivia/utils.hpp"

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

namespace builder {

TriviaBinder::TriviaBinder(antlr4::CommonTokenStream &ts) noexcept : tokens_(ts), used_(ts.size())
{
}

void TriviaBinder::collect(std::vector<ast::Trivia> &dst, std::span<antlr4::Token *const> tokens)
{
    unsigned int linebreaks{ 0 };

    auto process_linebreaks = [&dst, &linebreaks] -> void {
        if (linebreaks >= 2) {
            dst.emplace_back(ast::ParagraphBreak{ .blank_lines = linebreaks - 1 });
        }
        linebreaks = 0; // Reset after processing
    };

    for (const antlr4::Token *token : tokens) {
        const auto index = token->getTokenIndex();

        if (used_[index]) {
            continue;
        }
        used_[index] = true;

        if (isNewline(token)) {
            ++linebreaks;
            continue;
        }

        if (isComment(token)) {
            process_linebreaks();
            dst.emplace_back(ast::Comment{ token->getText() });
        }
    }

    // Process any remaining linebreaks at the end
    process_linebreaks();
}

auto TriviaBinder::collectInline(std::optional<ast::Comment> &dst, const std::size_t index)
{
    if (used_[index]) {
        return;
    }

    const auto &token = tokens_.get(index);

    if (isComment(token)) {
        used_[index] = true;
        dst.emplace(ast::Comment{ token->getText() });
        return; // only one inline comment per node
    }
}

void TriviaBinder::bind(ast::NodeBase &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &trivia = node.trivia.emplace();

    const auto start_index = ctx->getStart()->getTokenIndex();
    const auto stop_index = findLastDefaultOnLine(ctx->getStop()->getTokenIndex());

    collect(trivia.leading, tokens_.getHiddenTokensToLeft(start_index));
    collectInline(trivia.inline_comment, stop_index + 1);
    collect(trivia.trailing, tokens_.getHiddenTokensToRight(stop_index));
}

auto TriviaBinder::findLastDefaultOnLine(std::size_t start_index) const noexcept -> std::size_t
{
    const auto *start_token = tokens_.get(start_index);
    if (start_token == nullptr || !isDefault(start_token)) {
        return start_index;
    }

    const auto line = start_token->getLine();

    std::size_t result = start_index;

    for (std::size_t i = start_index + 1;; ++i) {
        const auto *token = tokens_.get(i);

        if (token == nullptr || token->getLine() != line) {
            break;
        }

        if (isDefault(token)) {
            result = i;
        }
    }

    return result;
}

} // namespace builder
