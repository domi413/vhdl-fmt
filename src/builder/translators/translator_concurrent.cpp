#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <ranges>

namespace builder {

auto Translator::makeConcurrentAssignStatement(
  vhdlParser::Concurrent_signal_assignment_statementContext *ctx)
  -> ast::ConcurrentSignalAssignmentStatement
{
    // Dispatch based on concrete assignment type
    if (auto *cond = ctx->conditional_signal_assignment()) {
        return makeConditionalAssignStatement(cond);
    }

    if (auto *sel = ctx->selected_signal_assignment()) {
        return makeSelectedAssign(sel);
    }

    // Fallback for unhandled cases
    return make<ast::ConcurrentSignalAssignmentStatement>(ctx);
}

auto Translator::makeConditionalAssignStatement(
  vhdlParser::Conditional_signal_assignmentContext *ctx) -> ast::ConcurrentSignalAssignmentStatement
{
    auto assign = make<ast::ConcurrentSignalAssignmentStatement>(ctx);

    if (auto *target_ctx = ctx->target()) {
        assign.target = makeTarget(target_ctx);
    }

    // Get the waveform - for now we'll take the first waveform element's expression
    auto *cond_wave = ctx->conditional_waveforms();
    if (cond_wave == nullptr) {
        return assign;
    }

    auto *wave = cond_wave->waveform();
    if (wave == nullptr) {
        return assign;
    }

    auto wave_elems = wave->waveform_element();
    if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
        assign.value = makeExpression(wave_elems[0]->expression(0));
    }

    return assign;
}

auto Translator::makeSelectedAssign(vhdlParser::Selected_signal_assignmentContext *ctx)
  -> ast::ConcurrentSignalAssignmentStatement
{
    auto assign = make<ast::ConcurrentSignalAssignmentStatement>(ctx);

    if (auto *target_ctx = ctx->target()) {
        assign.target = makeTarget(target_ctx);
    }

    // For selected assignments (with...select), we'll take the first waveform
    // TODO(someone): Handle full selected waveforms structure
    if (auto *sel_waves = ctx->selected_waveforms()) {
        auto waves = sel_waves->waveform();
        if (!waves.empty()) {
            auto wave_elems = waves[0]->waveform_element();
            if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
                assign.value = makeExpression(wave_elems[0]->expression(0));
            }
        }
    }

    return assign;
}

auto Translator::makeProcessStatement(vhdlParser::Process_statementContext *ctx)
  -> ast::ProcessStatement
{
    auto proc{ make<ast::ProcessStatement>(ctx) };

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            proc.label = id->getText();
        }
    }

    // Extract sensitivity list
    if (auto *sens_list = ctx->sensitivity_list()) {
        proc.sensitivity_list = sens_list->name()
                              | std::views::transform([](auto *name) { return name->getText(); })
                              | std::ranges::to<std::vector>();
    }

    // Extract sequential statements
    if (auto *stmt_part = ctx->process_statement_part()) {
        proc.body
          = stmt_part->sequential_statement()
          | std::views::transform([this](auto *stmt) { return makeSequentialStatement(stmt); })
          | std::ranges::to<std::vector>();
    }

    return proc;
}

} // namespace builder
