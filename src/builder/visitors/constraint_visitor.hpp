#ifndef BUILDER_CONSTRAINT_VISITOR_HPP
#define BUILDER_CONSTRAINT_VISITOR_HPP

#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"
#include "vhdlParser.h"

#include <any>
#include <vector>

namespace builder {

class ConstraintVisitor final : public TypedVisitor<std::vector<ast::BinaryExpr>>
{
    Translator &trans_;

  public:
    explicit ConstraintVisitor(Translator &trans) : trans_(trans) {}

  private:
    auto visitIndex_constraint(vhdlParser::Index_constraintContext *ctx) -> std::any override
    {
        setResult(trans_.makeIndexConstraint(ctx));
        return {};
    }

    auto visitRange_constraint(vhdlParser::Range_constraintContext *ctx) -> std::any override
    {
        setResult(trans_.makeRangeConstraint(ctx));
        return {};
    }
};

} // namespace builder

#endif // BUILDER_CONSTRAINT_VISITOR_HPP
