#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

namespace ast {

// Forward declarations
struct Node;
struct DesignFile;
struct DesignUnit;

struct Declaration;
struct ConstantDecl;
struct SignalDecl;
struct GenericParam;
struct Port;

struct Statement;
struct ConcurrentAssign;
struct Process;

struct Expr;
struct TokenExpr;
struct GroupExpr;
struct UnaryExpr;
struct BinaryExpr;
struct ParenExpr;

struct GenericClause;
struct PortClause;
struct Entity;
struct Architecture;

/// @brief Abstract interface for visiting AST nodes.
///
/// Defines the entry points for all visitable node types.
/// Concrete visitors implement these to perform operations
/// such as formatting, analysis, or transformation.
///
/// @tparam ReturnType The type returned by visit methods (void by default)
template<typename ReturnType = void>
class Visitor
{
  public:
    Visitor() = default;
    virtual ~Visitor() = default;

    Visitor(const Visitor &) = delete;
    auto operator=(const Visitor &) -> Visitor & = delete;
    Visitor(Visitor &&) = delete;
    auto operator=(Visitor &&) -> Visitor & = delete;

    // Core hierarchy
    virtual auto visit(const DesignFile &) -> ReturnType = 0;
    virtual auto visit(const DesignUnit &) -> ReturnType = 0;

    // Declarations
    virtual auto visit(const Declaration &) -> ReturnType = 0;
    virtual auto visit(const ConstantDecl &) -> ReturnType = 0;
    virtual auto visit(const SignalDecl &) -> ReturnType = 0;
    virtual auto visit(const GenericParam &) -> ReturnType = 0;
    virtual auto visit(const Port &) -> ReturnType = 0;

    // Structural clauses (used inside Entity/Architecture)
    virtual auto visit(const GenericClause &) -> ReturnType = 0;
    virtual auto visit(const PortClause &) -> ReturnType = 0;
    virtual auto visit(const Entity &) -> ReturnType = 0;
    virtual auto visit(const Architecture &) -> ReturnType = 0;

    // Statements
    virtual auto visit(const Statement &) -> ReturnType = 0;
    virtual auto visit(const ConcurrentAssign &) -> ReturnType = 0;
    virtual auto visit(const Process &) -> ReturnType = 0;

    // Expressions
    virtual auto visit(const Expr &) -> ReturnType = 0;
    virtual auto visit(const TokenExpr &node) -> ReturnType = 0;
    virtual auto visit(const GroupExpr &node) -> ReturnType = 0;
    virtual auto visit(const UnaryExpr &node) -> ReturnType = 0;
    virtual auto visit(const BinaryExpr &node) -> ReturnType = 0;
    virtual auto visit(const ParenExpr &node) -> ReturnType = 0;
};

} // namespace ast

#endif /* AST_VISITOR_HPP */
