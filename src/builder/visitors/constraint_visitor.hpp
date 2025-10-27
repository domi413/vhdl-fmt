#ifndef BUILDER_CONSTRAINT_VISITOR_HPP
#define BUILDER_CONSTRAINT_VISITOR_HPP

#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"
#include "vhdlParser.h"

#include <any>
#include <optional>
#include <vector>

namespace builder {

class ConstraintVisitor final : public TypedVisitor<ConstraintVisitor, std::vector<ast::BinaryExpr>>
{
    Translator &trans_;

  public:
    explicit ConstraintVisitor(Translator &trans) : trans_(trans) {}

    static auto translate(Translator &trans, vhdlParser::ConstraintContext *ctx)
      -> std::optional<std::vector<ast::BinaryExpr>>
    {
        ConstraintVisitor visitor{ trans };
        return visitor.TypedVisitor::translate(ctx);
    }

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
