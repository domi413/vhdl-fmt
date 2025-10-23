#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

namespace ast {

// Forward declarations
struct NodeBase;
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
    virtual void visit(const DesignFile &) = 0;
    virtual void visit(const DesignUnit &) = 0;

    // Declarations
    virtual void visit(const Declaration &) = 0;
    virtual void visit(const ConstantDecl &) = 0;
    virtual void visit(const SignalDecl &) = 0;
    virtual void visit(const GenericParam &) = 0;
    virtual void visit(const Port &) = 0;

    // Structural clauses (used inside Entity/Architecture)
    virtual void visit(const GenericClause &) = 0;
    virtual void visit(const PortClause &) = 0;
    virtual void visit(const Entity &) = 0;
    virtual void visit(const Architecture &) = 0;

    // Statements
    virtual void visit(const Statement &) = 0;
    virtual void visit(const ConcurrentAssign &) = 0;
    virtual void visit(const Process &) = 0;

    // Expressions
    virtual void visit(const Expr &) = 0;
    virtual void visit(const TokenExpr &node) = 0;
    virtual void visit(const GroupExpr &node) = 0;
    virtual void visit(const UnaryExpr &node) = 0;
    virtual void visit(const BinaryExpr &node) = 0;
    virtual void visit(const ParenExpr &node) = 0;
};

} // namespace ast

#endif /* AST_VISITOR_HPP */
