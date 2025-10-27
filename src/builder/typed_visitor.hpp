#ifndef BUILDER_TYPED_VISITOR_HPP
#define BUILDER_TYPED_VISITOR_HPP

#include "vhdlParserBaseVisitor.h"

#include <optional>
#include <tree/ParseTree.h>
#include <utility>

namespace builder {

/// @brief Typed visitor for ANTLR parse trees  
/// Simplified pattern: removes CRTP, keeps setResult for move-only types
///
/// Usage:
///   class MyVisitor : public TypedVisitor<MyReturnType> {
///   public:
///       explicit MyVisitor(Translator& t) : trans_(t) {}
///   private:
///       auto visitSomeContext(SomeContext* ctx) -> std::any override {
///           setResult(computeResult(ctx));
///           return {};
///       }
///   };
///
///   // Call site:
///   auto result = MyVisitor{trans}.translate(ctx);
template <typename ReturnType>
class TypedVisitor : public vhdlParserBaseVisitor
{
  public:
    ~TypedVisitor() override = default;

    TypedVisitor(const TypedVisitor &) = delete;
    auto operator=(const TypedVisitor &) -> TypedVisitor & = delete;
    TypedVisitor(TypedVisitor &&) = delete;
    auto operator=(TypedVisitor &&) -> TypedVisitor & = delete;

    /// @brief Main entry point: visit tree and return typed result
    auto translate(antlr4::tree::ParseTree *tree) -> ReturnType
    {
        result_.reset();
        visit(tree); // ANTLR dispatch to derived class
        return std::move(*result_);
    }

  protected:
    TypedVisitor() = default;

    /// @brief Store result from visit methods (handles move-only types)
    void setResult(ReturnType &&value) { result_ = std::move(value); }
    void setResult(const ReturnType &value) { result_ = value; }

  private:
    std::optional<ReturnType> result_;
};

} // namespace builder

#endif // BUILDER_TYPED_VISITOR_HPP