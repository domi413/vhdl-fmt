#ifndef EMIT_DOC_HPP
#define EMIT_DOC_HPP

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace emit {

// Forward declarations
class DocImpl;

// Default indentation amount for << and >> operators
inline constexpr int DEFAULT_INDENT = 2;

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
    static auto hardline() -> Doc;

    // Combinators
    auto operator+(const Doc &other) const -> Doc;  ///> Direct concatenation
    auto operator&(const Doc &other) const -> Doc;  ///> Space concatenation
    auto operator/(const Doc &other) const -> Doc;  ///> Softline
    auto operator|(const Doc &other) const -> Doc;  ///> Hardline
    auto operator<<(const Doc &other) const -> Doc; ///> Line break + indent rhs
    auto operator>>(const Doc &other) const -> Doc; ///> Line break + dedent rhs

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
