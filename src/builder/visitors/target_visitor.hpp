#pragma once

#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"

namespace builder {

// CRTP-based visitor for assignment targets
//
// Translates VHDL target contexts (used in assignments) to AST expressions.
// Targets can be:
// - Names (signals, variables)
// - Aggregates (for multiple assignments)
//
// Usage:
//   auto target = TargetVisitor::translate(translator, target_ctx);
class TargetVisitor : public TypedVisitor<TargetVisitor, ast::Expr>
{
    Translator &trans_;

  public:
    explicit TargetVisitor(Translator &trans) : trans_(trans) {}

    // Static factory method for convenient usage
    static auto translate(Translator &trans, vhdlParser::TargetContext *ctx)
      -> std::optional<ast::Expr>
    {
        TargetVisitor visitor{ trans };
        return visitor.TypedVisitor::translate(ctx);
    }

  private:
    friend class TypedVisitor<TargetVisitor, ast::Expr>;

    // Target can be a name
    std::any visitName(vhdlParser::NameContext *ctx) override
    {
        setResult(trans_.makeName(ctx));
        return {};
    }

    // Target can be an aggregate
    std::any visitAggregate(vhdlParser::AggregateContext *ctx) override
    {
        setResult(trans_.makeAggregate(ctx));
        return {};
    }
};

} // namespace builder
