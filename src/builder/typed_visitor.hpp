#ifndef BUILDER_TYPED_VISITOR_HPP
#define BUILDER_TYPED_VISITOR_HPP

#include "vhdlParserBaseVisitor.h"

#include <optional>
#include <tree/ParseTree.h>

namespace builder {

// CRTP-based typed visitor for ANTLR parse trees
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

    TypedVisitor(const TypedVisitor &) = delete;
    auto operator=(const TypedVisitor &) -> TypedVisitor & = delete;
    TypedVisitor(TypedVisitor &&) = delete;
    auto operator=(TypedVisitor &&) -> TypedVisitor & = delete;

    // Main entry point: visit tree and return typed result
    auto translate(antlr4::tree::ParseTree *tree) -> std::optional<ReturnType>
    {
        result_value_.reset();
        visit(tree); // ANTLR dispatch
        return std::move(result_value_);
    }

  protected:
    TypedVisitor() = default;

    // Called by derived class visit methods
    void setResult(ReturnType &&value) { result_value_ = std::move(value); }
    void setResult(const ReturnType &value) { result_value_ = value; }

  private:
    std::optional<ReturnType> result_value_;
};

} // namespace builder

#endif // BUILDER_TYPED_VISITOR_HPP