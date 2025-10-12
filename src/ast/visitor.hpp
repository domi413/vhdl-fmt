#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

namespace ast {

struct DesignFile;
struct Entity;
struct GenericParam;
struct Range;
struct Port;

/**
 * @brief Interface used to traverse semantic context (AST nodes).
 *
 * The `ASTVisitor` walks the tree produced by the builder layer.
 *
 * Responsibilities:
 *  - Traverse semantic context (AST nodes)
 */
struct Visitor
{
    Visitor() = default;
    virtual ~Visitor() = default;

    Visitor(const Visitor &) = delete;
    auto operator=(const Visitor &) -> Visitor & = delete;
    Visitor(Visitor &&) = delete;
    auto operator=(Visitor &&) -> Visitor & = delete;

    // ---- Root ----
    virtual void visit(const DesignFile &) = 0;

    // ---- Declarations ----
    virtual void visit(const Entity &) = 0;
    virtual void visit(const GenericParam &) = 0;
    virtual void visit(const Port &) = 0;
    virtual void visit(const Range &) = 0;
};

} // namespace ast

#endif /* AST_VISITOR_HPP */
