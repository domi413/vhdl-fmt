#pragma once

#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"
#include "builder/visitors/target_visitor.hpp"

namespace builder {

// CRTP-based visitor for concurrent signal assignments
//
// Translates VHDL concurrent assignment contexts to AST ConcurrentAssign nodes.
// Handles two types:
// - Conditional signal assignments (with condition)
// - Selected signal assignments (with...select)
//
// Usage:
//   auto assign = ConcurrentAssignmentVisitor::translate(translator, ctx);
class ConcurrentAssignmentVisitor
  : public TypedVisitor<ConcurrentAssignmentVisitor, ast::ConcurrentAssign>
{
    Translator &trans_;

  public:
    explicit ConcurrentAssignmentVisitor(Translator &trans) : trans_(trans) {}

    // Static factory method for convenient usage
    static auto translate(Translator &trans,
                          vhdlParser::Concurrent_signal_assignment_statementContext *ctx)
      -> std::optional<ast::ConcurrentAssign>
    {
        ConcurrentAssignmentVisitor visitor{ trans };
        return visitor.TypedVisitor::translate(ctx);
    }

  private:
    friend class TypedVisitor<ConcurrentAssignmentVisitor, ast::ConcurrentAssign>;

    // Handle conditional signal assignment
    std::any visitConditional_signal_assignment(
      vhdlParser::Conditional_signal_assignmentContext *ctx) override
    {
        ast::ConcurrentAssign assign;

        if (auto *target_ctx = ctx->target()) {
            if (auto target = TargetVisitor::translate(trans_, target_ctx)) {
                assign.target = std::move(*target);
            }
        }

        // Get the waveform - for now we'll take the first waveform element's expression
        if (auto *cond_wave = ctx->conditional_waveforms()) {
            if (auto *wave = cond_wave->waveform()) {
                auto wave_elems = wave->waveform_element();
                if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
                    assign.value = trans_.makeExpr(wave_elems[0]->expression(0));
                }
            }
        }

        setResult(std::move(assign));
        return {};
    }

    // Handle selected signal assignment
    std::any visitSelected_signal_assignment(
      vhdlParser::Selected_signal_assignmentContext *ctx) override
    {
        ast::ConcurrentAssign assign;

        if (auto *target_ctx = ctx->target()) {
            if (auto target = TargetVisitor::translate(trans_, target_ctx)) {
                assign.target = std::move(*target);
            }
        }

        // For selected assignments (with...select), we'll take the first waveform
        // TODO(dyb): Handle full selected waveforms structure
        if (auto *sel_waves = ctx->selected_waveforms()) {
            auto waves = sel_waves->waveform();
            if (!waves.empty()) {
                auto wave_elems = waves[0]->waveform_element();
                if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
                    assign.value = trans_.makeExpr(wave_elems[0]->expression(0));
                }
            }
        }

        setResult(std::move(assign));
        return {};
    }
};

} // namespace builder
