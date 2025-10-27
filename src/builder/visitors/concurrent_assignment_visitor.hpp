#ifndef BUILDER_CONCURRENT_ASSIGNMENT_VISITOR_HPP
#define BUILDER_CONCURRENT_ASSIGNMENT_VISITOR_HPP

#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"
#include "vhdlParser.h"

#include <any>

namespace builder {

class ConcurrentAssignmentVisitor final : public TypedVisitor<ast::ConcurrentAssign>
{
    Translator &trans_;

  public:
    explicit ConcurrentAssignmentVisitor(Translator &trans) : trans_(trans) {}

  private:
    auto visitConditional_signal_assignment(vhdlParser::Conditional_signal_assignmentContext *ctx)
      -> std::any override
    {
        setResult(trans_.makeConditionalAssign(ctx));
        return {};
    }

    auto visitSelected_signal_assignment(vhdlParser::Selected_signal_assignmentContext *ctx)
      -> std::any override
    {
        setResult(trans_.makeSelectedAssign(ctx));
        return {};
    }
};

} // namespace builder

#endif // BUILDER_CONCURRENT_ASSIGNMENT_VISITOR_HPP