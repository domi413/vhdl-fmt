#include "ast/nodes/declarations.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

void Translator::makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
{
    spawn<ast::GenericParam>(ctx, true, [&](auto &param) {
        for (auto *id_ctx : ctx->identifier_list()->identifier()) {
            param.names.emplace_back(id_ctx->getText());
        }

        if (auto *stype = ctx->subtype_indication()) {
            param.type_name = stype->getText();
        }
    });
}

void Translator::makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx)
{
    spawn<ast::Port>(ctx, true, [&](auto &port) {
        const auto &ids = ctx->identifier_list()->identifier();
        port.names.reserve(ids.size());

        for (auto *id_ctx : ctx->identifier_list()->identifier()) {
            port.names.emplace_back(id_ctx->getText());
        }

        if (auto *mode = ctx->signal_mode()) {
            port.mode = mode->getText();
        }

        if (auto *stype = ctx->subtype_indication()) {
            port.type_name = stype->selected_name(0)->getText();

            if (auto *constraint = stype->constraint()) {
                into(port.constraints, [&] { dispatch_(constraint); });
            }
        }

        if (auto *expr = ctx->expression()) {
            into(port.default_expr, [&] { dispatch_(expr); });
        }
    });
}

void Translator::makeConstantDecl(vhdlParser::Constant_declarationContext *ctx)
{
    spawn<ast::ConstantDecl>(ctx, true, [&](auto &decl) {
        for (auto *id_ctx : ctx->identifier_list()->identifier()) {
            decl.names.emplace_back(id_ctx->getText());
        }

        if (auto *stype = ctx->subtype_indication()) {
            decl.type_name = stype->selected_name(0)->getText();
        }

        if (auto *expr = ctx->expression()) {
            into(decl.init_expr, [&] { dispatch_(expr); });
        }
    });
}

void Translator::makeSignalDecl(vhdlParser::Signal_declarationContext *ctx)
{
    spawn<ast::SignalDecl>(ctx, true, [&](auto &decl) {
        for (auto *id_ctx : ctx->identifier_list()->identifier()) {
            decl.names.emplace_back(id_ctx->getText());
        }

        if (auto *stype = ctx->subtype_indication()) {
            decl.type_name = stype->selected_name(0)->getText();
        }

        decl.has_bus_kw = false;
        if (auto *kind = ctx->signal_kind()) {
            if (kind->BUS() != nullptr) {
                decl.has_bus_kw = true;
            }
        }

        if (auto *stype = ctx->subtype_indication()) {
            if (auto *constraint = stype->constraint()) {
                into(decl.constraints, [&] { dispatch_(constraint); });
            }
        }

        if (auto *expr = ctx->expression()) {
            into(decl.init_expr, [&] { dispatch_(expr); });
        }
    });
}

} // namespace builder