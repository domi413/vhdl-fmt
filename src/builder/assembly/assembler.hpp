#ifndef BUILDER_ASSEMBLY_ASSEMBLER_HPP
#define BUILDER_ASSEMBLY_ASSEMBLER_HPP

#include "ast/node.hpp"

#include <cassert>
#include <concepts>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace builder {

/**
 * @brief Assembler for AST construction.
 *
 * The `Assembler` manages node creation and insertion into active
 * AST containers (sinks). It provides scoped control over where new
 * nodes are appended during AST assembly.
 */
class Assembler
{
  public:
    // Construct attached to any sink (e.g., DesignFile.units)
    template<typename Vec>
    explicit Assembler(Vec &sink)
    {
        pushSink(sink);
    }

    ~Assembler() noexcept { assert(sinks.size() == 1 && "Unclosed sinks remain!"); }

    Assembler(const Assembler &) = delete;
    auto operator=(const Assembler &) -> Assembler & = delete;
    Assembler(Assembler &&) = delete;
    auto operator=(Assembler &&) -> Assembler & = delete;

    // --- Core interface ---
    template<typename T>
    [[nodiscard]] auto spawn() -> T &
    {
        static_assert(std::derived_from<T, ast::Node>, "T must derive from ast::Node");
        assert(!sinks.empty() && "No active sink!");

        auto node = std::make_unique<T>();
        T &ref = *node;
        sinks.back()(std::move(node)); // Push through the active sink
        return ref;
    }

    template<typename Vec>
    [[nodiscard]] auto with(Vec &vec)
    {
        pushSink(vec);
        return Scope{ *this };
    }

    template<typename Vec, typename Fn>
    void into(Vec &vec, Fn &&fn)
    {
        auto scope = with(vec);
        std::forward<Fn>(fn)();
    }

  private:
    using SinkFn = std::function<void(std::unique_ptr<ast::Node>)>;
    std::vector<SinkFn> sinks;

    // RAII scope
    struct Scope
    {
        explicit Scope(Assembler &a) : assembler(a) {}
        ~Scope() { assembler.sinks.pop_back(); }

        Scope(const Scope &) = delete;
        auto operator=(const Scope &) -> Scope & = delete;
        Scope(Scope &&) = delete;
        auto operator=(Scope &&) -> Scope & = delete;

      private:
        Assembler &assembler;
    };

    // Push a new sink that can accept any derived type
    template<typename Vec>
    void pushSink(Vec &vec)
    {
        using ElemT = typename Vec::value_type::element_type;
        static_assert(std::derived_from<ElemT, ast::Node>,
                      "Sink element type must derive from ast::Node");

        sinks.emplace_back([&vec](std::unique_ptr<ast::Node> n) {
            auto casted = std::unique_ptr<ElemT>(static_cast<ElemT *>(n.release()));
            vec.push_back(std::move(casted));
        });
    }
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_ASSEMBLER_HPP */
