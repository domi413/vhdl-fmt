#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

namespace ast {

struct Node;
struct DesignFile;
struct Entity;
struct GenericParam;
struct Range;
struct Port;

/**
 * @brief Abstract interface for visiting AST nodes.
 *
 * Defines the entry points for all visitable node types.
 * Concrete visitors implement these to perform operations
 * on specific parts of the AST.
 *
 * **Responsibilities:**
 *  - Define visit entry points for all node types.
 */
class Visitor
{
  public:
    Visitor() = default;
    virtual ~Visitor() = default;

    Visitor(const Visitor &) = delete;
    auto operator=(const Visitor &) -> Visitor & = delete;
    Visitor(Visitor &&) = delete;
    auto operator=(Visitor &&) -> Visitor & = delete;

    // Node visitors
    virtual void visit(const DesignFile &) = 0;
    virtual void visit(const Entity &) = 0;
    virtual void visit(const GenericParam &) = 0;
    virtual void visit(const Port &) = 0;
    virtual void visit(const Range &) = 0;
};

} // namespace ast

#endif /* AST_VISITOR_HPP */
