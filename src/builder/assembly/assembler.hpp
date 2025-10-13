#ifndef BUILDER_ASSEMBLY_ASSEMBLER_HPP
#define BUILDER_ASSEMBLY_ASSEMBLER_HPP

#include "ast/node.hpp"
#include "builder/assembly/sink.hpp"

#include <cassert>
#include <concepts>
#include <memory>
#include <utility>
#include <vector>

namespace builder {

/// @brief Hierarchical AST node assembler.
///
/// Manages scoped insertion of `ast::Node`-derived objects into
/// heterogeneously-typed containers (e.g. `std::vector<std::unique_ptr<T>>`).
class Assembler
{
  public:
    /// @brief Initialize assembler with a root sink (e.g. `DesignFile.units`).
    template<typename Vec>
    explicit Assembler(Vec &root)
    {
        pushSink(root);
    }

    ~Assembler() noexcept { assert(sinks.size() == 1 && "Unclosed sinks remain!"); }

    Assembler(const Assembler &) = delete;
    auto operator=(const Assembler &) -> Assembler & = delete;
    Assembler(Assembler &&) = delete;
    auto operator=(Assembler &&) -> Assembler & = delete;

    /// @brief Create and insert a new node into the active sink.
    /// @return Reference to the created node.
    template<std::derived_from<ast::Node> T>
    [[nodiscard]] auto spawn() -> T &
    {
        assert(!sinks.empty() && "No active sink!");
        auto node = std::make_unique<T>();
        T &ref = *node;
        sinks.back()->push(std::move(node));
        return ref;
    }

    /// @brief Temporarily redirect insertions into another container.
    template<typename Vec, typename Fn>
    void into(Vec &vec, Fn &&fn)
    {
        pushSink(vec);
        std::forward<Fn>(fn)();
        sinks.pop_back();
    }

  private:
    /// @brief Stack of active sinks.
    std::vector<std::unique_ptr<ISink>> sinks;

    /// @brief Push a new sink for the given container.
    template<typename Vec>
    void pushSink(Vec &vec)
    {
        using ElemT = typename Vec::value_type::element_type;
        static_assert(std::derived_from<ElemT, ast::Node>,
                      "Sink element type must derive from ast::Node");
        sinks.push_back(std::make_unique<Sink<ElemT>>(vec));
    }
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_ASSEMBLER_HPP */
