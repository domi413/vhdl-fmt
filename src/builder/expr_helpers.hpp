#ifndef BUILDER_EXPR_HELPERS_HPP
#define BUILDER_EXPR_HELPERS_HPP

#include "ast/nodes/expressions.hpp"
#include "builder/trivia/trivia_binder.hpp"

#include <memory>
#include <string>
#include <utility>

namespace builder::expr_helpers {

/// @brief Helper to create a boxed expression
template<typename T = ast::Expr>
[[nodiscard]]
inline auto box(T &&expr) -> ast::Box<T>
{
    return std::make_unique<T>(std::forward<T>(expr));
}

/// @brief Helper to create binary expressions
template<typename Ctx>
[[nodiscard]]
inline auto makeBinary(TriviaBinder &trivia,
                       Ctx *ctx,
                       std::string op,
                       ast::Expr left,
                       ast::Expr right) -> ast::Expr
{
    ast::BinaryExpr bin;
    trivia.bind(bin, ctx);
    bin.op = std::move(op);
    bin.left = box(std::move(left));
    bin.right = box(std::move(right));
    return bin;
}

/// @brief Helper to create unary expressions
template<typename Ctx>
[[nodiscard]]
inline auto makeUnary(TriviaBinder &trivia, Ctx *ctx, std::string op, ast::Expr value) -> ast::Expr
{
    ast::UnaryExpr un;
    trivia.bind(un, ctx);
    un.op = std::move(op);
    un.value = box(std::move(value));
    return un;
}

/// @brief Helper to create token expressions
template<typename Ctx>
[[nodiscard]]
inline auto makeToken(TriviaBinder &trivia, Ctx *ctx, std::string text) -> ast::Expr
{
    ast::TokenExpr tok;
    trivia.bind(tok, ctx);
    tok.text = std::move(text);
    return tok;
}

} // namespace builder::expr_helpers

#endif /* BUILDER_EXPR_HELPERS_HPP */
