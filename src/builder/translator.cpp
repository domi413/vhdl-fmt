#include "translator.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <cstddef>
#include <vector>

namespace builder {

auto Translator::makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity &
{
    auto &entity{ spawn<ast::Entity>(ctx) };
    entity.name = ctx->identifier(0)->getText();
    return entity;
}

auto Translator::makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
  -> ast::GenericParam &
{
    auto &param = spawn<ast::GenericParam>(ctx);
    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        param.names.push_back(id_ctx->getText());
    }

    param.type = ctx->subtype_indication()->getText();
    if (ctx->expression() != nullptr) {
        param.init = ctx->expression()->getText();
    }
    return param;
}

auto Translator::makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port &
{
    auto &port = spawn<ast::Port>(ctx);

    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        port.names.push_back(id_ctx->getText());
    }

    if (ctx->signal_mode() != nullptr) {
        port.mode = ctx->signal_mode()->getText();
    }

    if (auto *stype = ctx->subtype_indication()) {
        if (!stype->selected_name().empty()) {
            port.type = stype->selected_name(0)->getText();
        }
    }

    // Optional initializer
    if (ctx->expression() != nullptr) {
        port.init = ctx->expression()->getText();
    }

    return port;
}

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Range &
{
    auto &range = spawn<ast::Range>(ctx);
    range.left_expr = ctx->simple_expression(0)->getText();
    range.direction = ctx->direction()->getText();
    range.right_expr = ctx->simple_expression(1)->getText();

    return range;
}

void Translator::attachComments(ast::Node &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }
    auto &cm = node.getComments();
    auto kind = [](const antlr4::Token *t) -> ast::Comment::Kind {
        return t->getType() == vhdlLexer::COMMENT ? ast::Comment::Kind::line
                                                  : ast::Comment::Kind::block;
    };
    const auto push
      = [&](const antlr4::Token *t, std::vector<ast::Comment> &dst, bool is_inline) -> void {
        if (!t) {
            return;
        }
        const std::size_t idx{ t->getTokenIndex() };
        if (consumed_comment_token_indices.contains(idx)) {
            return;
        }
        consumed_comment_token_indices.insert(idx);
        dst.push_back({ t->getText(), kind(t), static_cast<int>(t->getLine()), is_inline });
    };

    // Leading: everything hidden to the left of start (consume all)
    for (const auto *t : tokens.getHiddenTokensToLeft(ctx->getStart()->getTokenIndex())) {
        push(t, cm.leading, false);
    }

    // Trailing: only inline. Standalone comments are left
    // unconsumed here so the next node will pick them up as leading.
    const std::size_t stop_line = ctx->getStop()->getLine();
    for (const auto *t : tokens.getHiddenTokensToRight(ctx->getStop()->getTokenIndex())) {
        if (t == nullptr) {
            continue;
        }
        if (t->getLine() == stop_line) {
            push(t, cm.trailing, true);
        }
    }
}

} // namespace builder
