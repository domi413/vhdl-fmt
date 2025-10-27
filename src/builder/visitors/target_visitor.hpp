#ifndef BUILDER_TARGET_VISITOR_HPP
#define BUILDER_TARGET_VISITOR_HPP

#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"
#include "vhdlParser.h"

#include <any>

namespace builder {

class TargetVisitor final : public TypedVisitor<ast::Expr>
{
    Translator &trans_;

  public:
    explicit TargetVisitor(Translator &trans) : trans_(trans) {}

  private:
    auto visitName(vhdlParser::NameContext *ctx) -> std::any override
    {
        setResult(trans_.makeName(ctx));
        return {};
    }

    auto visitAggregate(vhdlParser::AggregateContext *ctx) -> std::any override
    {
        setResult(trans_.makeAggregate(ctx));
        return {};
    }
};

} // namespace builder

#endif // BUILDER_TARGET_VISITOR_HPP