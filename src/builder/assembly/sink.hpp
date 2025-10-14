#ifndef BUILDER_ASSEMBLY_SINK_HPP
#define BUILDER_ASSEMBLY_SINK_HPP

#include "ast/node.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace builder {

/// @brief Abstract interface for AST node sinks.
///
/// Provides a uniform way to accept `std::unique_ptr<ast::Node>` objects,
/// regardless of their concrete container type.
struct ISink
{
    /// @brief Default constructor required for derived sink initialization.
    ISink() = default;
    virtual ~ISink() = default;

    ISink(const ISink &) = delete;
    auto operator=(const ISink &) -> ISink & = delete;
    ISink(ISink &&) = delete;
    auto operator=(ISink &&) -> ISink & = delete;

    /// @brief Insert a node into the underlying container.
    virtual void push(std::unique_ptr<ast::Node>) = 0;
};

/// @brief Type-specific sink for containers of derived nodes.
///
/// Adapts a `std::vector<std::unique_ptr<ElemT>>` to the `ISink` interface,
/// casting incoming `ast::Node` pointers to the correct element type before insertion.
template<typename ElemT>
struct Sink : ISink
{
    /// @brief Bind this sink to an external node container.
    explicit Sink(std::vector<std::unique_ptr<ElemT>> &v) : vec_(v) {}

    /// @brief Store a node in the bound container after casting.
    void push(std::unique_ptr<ast::Node> n) override
    {
        auto casted = std::unique_ptr<ElemT>(static_cast<ElemT *>(n.release()));
        vec_.emplace_back(std::move(casted));
    }

  private:
    /// @brief Reference to the target container.
    std::vector<std::unique_ptr<ElemT>> &vec_;
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_SINK_HPP */
