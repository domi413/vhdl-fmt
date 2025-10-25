#pragma once

#include "vhdlParserBaseVisitor.h"

#include <optional>

namespace builder {

// CRTP-based typed visitor for ANTLR parse trees
//
// This template provides a type-safe wrapper around ANTLR's visitor pattern.
// Instead of returning std::any everywhere, you specify your actual return type
// and use setResult() to populate it in a type-safe manner.
//
// Benefits:
// ✓ Type safety: Compile-time checking of return types
// ✓ Clean API: Public translate() returns the actual type, not std::any
// ✓ Zero overhead: Uses ANTLR's existing virtual dispatch
// ✓ Reusable: Works for any ANTLR visitor pattern
//
// Usage pattern:
//   class MyVisitor : public TypedVisitor<MyVisitor, MyReturnType> {
//   public:
//       static auto translate(Translator& t, Context* ctx) {
//           MyVisitor v{t};
//           return v.TypedVisitor::translate(ctx);
//       }
//   private:
//       std::any visitSomeContext(SomeContext* ctx) override {
//           setResult(computeResult(ctx));
//           return {};
//       }
//   };
template<typename Derived, typename ReturnType>
class TypedVisitor : public vhdlParserBaseVisitor
{
  public:
    ~TypedVisitor() override = default;

    // Not copyable - visitors are single-use with reference members
    TypedVisitor(const TypedVisitor &) = delete;
    auto operator=(const TypedVisitor &) -> TypedVisitor & = delete;
    
    // Not movable - no need to move, used immediately after construction
    TypedVisitor(TypedVisitor &&) = delete;
    auto operator=(TypedVisitor &&) -> TypedVisitor & = delete;

    // Main entry point: visit tree and return typed result
    //
    // This calls ANTLR's visit() which dispatches to visitXXX() methods.
    // The derived class's visitXXX() methods should call setResult() to
    // populate result with the translated value.
    auto translate(antlr4::tree::ParseTree *tree) -> std::optional<ReturnType>
    {
        result_value_.reset();
        visit(tree); // ANTLR's dispatch magic happens here
        return std::move(result_value_);
    }

  protected:
    // Protected constructor - only derived classes can instantiate
    TypedVisitor() = default;

    // Type-safe result setter - called by derived class visit methods
    void setResult(ReturnType &&value) { result_value_ = std::move(value); }

    void setResult(const ReturnType &value) { result_value_ = value; }

  private:
    std::optional<ReturnType> result_value_;
};

} // namespace builder
