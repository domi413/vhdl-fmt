#include "translator.hpp"

#include "ast/nodes/declarations.hpp"
#include "vhdlParser.h"

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
} // namespace builder
