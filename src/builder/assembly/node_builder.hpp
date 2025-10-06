#ifndef BUILDER_ASSEMBLY_NODE_BUILDER_HPP
#define BUILDER_ASSEMBLY_NODE_BUILDER_HPP

#include "ast/node.hpp"
#include "builder/assembly/builder_scope.hpp"
#include "builder/assembly/builder_sink.hpp"

#include <cassert>
#include <concepts>
#include <memory>
#include <vector>

namespace builder {

/**
 * @brief Assembles AST nodes into hierarchical structures.
 *
 * The `NodeAssembler` manages node creation and insertion into active
 * AST containers (sinks). It provides scoped control over where new
 * nodes are appended during AST assembly.
 *
 * Responsibilities:
 *  - Spawn and insert new AST nodes
 *  - Manage nested insertion scopes via `with` / `into`
 *  - Ensure sink stack integrity during construction
 *
 * Contains no parsing or traversal logic.
 */
struct NodeAssembler
{
    using Node = ast::Node;
    using NodePtr = std::unique_ptr<Node>;

    // Attach to any node container (e.g. DesignFile.units)
    explicit NodeAssembler(std::vector<std::unique_ptr<ast::Node>> &sink)
    {
        sinks.push_back(std::make_unique<SinkImpl<ast::Node>>(sink));
    }

    ~NodeAssembler() noexcept { assert(sinks.size() <= 1 && "Unclosed sinks remain!"); }

    NodeAssembler(const NodeAssembler &) = delete;
    auto operator=(const NodeAssembler &) -> NodeAssembler & = delete;
    NodeAssembler(NodeAssembler &&) = delete;
    auto operator=(NodeAssembler &&) -> NodeAssembler & = delete;

    // Spawn node and insert into current sink
    template<typename T, typename... Args>
    [[nodiscard]] auto spawn(Args &&...args) -> T &
    {
        static_assert(std::derived_from<T, Node>, "T must derive from ast::Node");
        assert(!sinks.empty() && "No active sink");

        auto node = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *node;
        sinks.back()->push(std::move(node));
        return ref;
    }

    template<typename Vec>
    auto with(Vec &vec) -> SlotGuard<Vec>
    {
        return SlotGuard<Vec>(sinks, vec);
    }

    template<typename Vec, typename Fn>
    void into(Vec &vec, Fn &&fn)
    {
        auto g = with(vec);
        std::forward<Fn>(fn)();
    }

  private:
    std::vector<std::unique_ptr<ISink>> sinks; // stack of active sinks
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_NODE_BUILDER_HPP */
