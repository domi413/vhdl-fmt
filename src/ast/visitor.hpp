#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

namespace ast {

struct DesignFile;
struct Entity;
struct GenericParam;
struct Range;
struct Port;

struct ASTVisitor
{
    ASTVisitor() = default;
    virtual ~ASTVisitor() = default;

    ASTVisitor(const ASTVisitor &) = delete;
    auto operator=(const ASTVisitor &) -> ASTVisitor & = delete;
    ASTVisitor(ASTVisitor &&) = delete;
    auto operator=(ASTVisitor &&) -> ASTVisitor & = delete;

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
