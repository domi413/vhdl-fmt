#pragma once
#include "ast/node.hpp"
#include "builder/assembly/builder_scope.hpp"
#include "builder/assembly/builder_sink.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace builder {

struct ASTBuilder
{
    using Node = ast::Node;
    using NodePtr = std::unique_ptr<Node>;

    // Attach to any node container (e.g. DesignFile.units)
    explicit ASTBuilder(std::vector<std::unique_ptr<ast::Node>> &sink)
    {
        sinks.push_back(std::make_unique<SinkImpl<ast::Node>>(sink));
    }

    ~ASTBuilder() noexcept { assert(sinks.size() <= 1 && "Unclosed sinks remain!"); }

    ASTBuilder(const ASTBuilder &) = delete;
    auto operator=(const ASTBuilder &) -> ASTBuilder & = delete;
    ASTBuilder(ASTBuilder &&) = delete;
    auto operator=(ASTBuilder &&) -> ASTBuilder & = delete;

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
