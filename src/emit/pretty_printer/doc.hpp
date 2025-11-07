#ifndef EMIT_DOC_HPP
#define EMIT_DOC_HPP

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace emit {

// Forward declarations
class DocImpl;

/// Document abstraction for pretty printing
class Doc final
{
    std::shared_ptr<DocImpl> impl_;
    explicit Doc(std::shared_ptr<DocImpl> impl) : impl_(std::move(impl)) {}

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

    // Combinators
    auto operator+(const Doc &other) const -> Doc;  ///> Direct concatenation
    auto operator&(const Doc &other) const -> Doc;  ///> Space concatenation
    auto operator/(const Doc &other) const -> Doc;  ///> Line break
    auto operator<<(const Doc &other) const -> Doc; ///> Line break + indent rhs
    auto operator>>(const Doc &other) const -> Doc; ///> Line break + dedent rhs
    [[nodiscard]]
    auto nest(int indent) const -> Doc;
    [[nodiscard]]
    auto group() const -> Doc;

    // Rendering
    [[nodiscard]]
    auto render(int width) const -> std::string;

    // Internal access
    [[nodiscard]]
    auto impl() const -> const std::shared_ptr<DocImpl> &;
};

// Helper functions
auto space() -> Doc;

} // namespace emit

#endif // EMIT_DOC_HPP
