#ifndef EMIT_DOC_HPP
#define EMIT_DOC_HPP

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace common {
struct Config;
} // namespace common

namespace emit {

// Forward declarations
class DocImpl;
using DocPtr = std::shared_ptr<DocImpl>;

template<typename Fn>
auto transformRecursive(const DocPtr &doc, Fn &&fn) -> DocPtr;

template<typename T, typename Fn>
auto foldRecursive(const DocPtr &doc, T init, Fn &&fn) -> T;

/// Document abstraction for pretty printing
class Doc final
{
  public:
    Doc();
    ~Doc() = default;

    Doc(const Doc &) = default;
    auto operator=(const Doc &) -> Doc & = default;
    Doc(Doc &&) noexcept = default;
    auto operator=(Doc &&) noexcept -> Doc & = default;

    // Basic constructors
    static auto empty() -> Doc;
    static auto text(std::string_view str) -> Doc;
    static auto line() -> Doc;
    static auto hardline() -> Doc;
    static auto alignText(std::string_view str) -> Doc;

    // Combinators
    auto operator+(const Doc &other) const -> Doc;  ///< Direct concatenation
    auto operator&(const Doc &other) const -> Doc;  ///< Space concatenation
    auto operator/(const Doc &other) const -> Doc;  ///< Softline
    auto operator|(const Doc &other) const -> Doc;  ///< Hardline
    auto operator<<(const Doc &other) const -> Doc; ///< Softline + indent rhs

    [[nodiscard]]
    auto hardIndent(const Doc &other) const -> Doc; ///< Hardline + indent rhs

    // Compound assignment operators
    auto operator+=(const Doc &other) -> Doc &;  ///< Direct concatenation assignment
    auto operator&=(const Doc &other) -> Doc &;  ///< Space concatenation assignment
    auto operator/=(const Doc &other) -> Doc &;  ///< Softline assignment
    auto operator|=(const Doc &other) -> Doc &;  ///< Hardline assignment
    auto operator<<=(const Doc &other) -> Doc &; ///< Softline + indent rhs assignment

    // Higher-level combinators for common patterns
    [[nodiscard]]
    static auto bracket(const Doc &left, const Doc &inner, const Doc &right) -> Doc;

    [[nodiscard]]
    auto group() const -> Doc;

    /// Transform document recursively using a callable that accepts each node type
    template<typename Fn>
    auto transform(Fn &&fn) const -> Doc
    {
        return Doc(transformRecursive(impl_, std::forward<Fn>(fn)));
    }

    /// Fold (reduce) the document tree into a single value.
    /// @param init The initial value for the accumulator.
    /// @param fn A callable: T f(T accumulator, const auto& node_variant)
    template<typename T, typename Fn>
    auto fold(T init, Fn &&fn) const -> T
    {
        return foldRecursive(impl_, std::move(init), std::forward<Fn>(fn));
    }

    [[nodiscard]]
    static auto resolveAlignment(const Doc &doc) -> Doc;

    // Rendering
    [[nodiscard]]
    auto render(const common::Config &config) const -> std::string;

  private:
    std::shared_ptr<DocImpl> impl_;
    explicit Doc(std::shared_ptr<DocImpl> impl) : impl_(std::move(impl)) {}
};

} // namespace emit

#endif // EMIT_DOC_HPP
