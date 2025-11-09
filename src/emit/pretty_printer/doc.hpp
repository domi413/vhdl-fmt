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

// Default indentation amount for << and >> operators (in indent levels, not spaces)
inline constexpr int DEFAULT_INDENT = 1;

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

    // Combinators
    auto operator+(const Doc &other) const -> Doc;  ///< Direct concatenation
    auto operator&(const Doc &other) const -> Doc;  ///< Space concatenation
    auto operator/(const Doc &other) const -> Doc;  ///< Softline
    auto operator|(const Doc &other) const -> Doc;  ///< Hardline
    auto operator<<(const Doc &other) const -> Doc; ///< Softline + indent rhs
    auto operator>>(const Doc &other) const -> Doc; ///< Softline + dedent rhs

    [[nodiscard]]
    auto hardIndent(const Doc &other) const -> Doc; ///< Hardline + indent rhs
    [[nodiscard]]
    auto hardDedent(const Doc &other) const -> Doc; ///< Hardline + dedent rhs

    // Higher-level combinators for common patterns
    [[nodiscard]]
    static auto bracket(const Doc &left, const Doc &inner, const Doc &right) -> Doc;

    [[nodiscard]]
    auto group() const -> Doc;

    // Rendering
    [[nodiscard]]
    auto render(const common::Config &config) const -> std::string;

  private:
    std::shared_ptr<DocImpl> impl_;
    explicit Doc(std::shared_ptr<DocImpl> impl) : impl_(std::move(impl)) {}
};

// Helper functions
auto space() -> Doc;

} // namespace emit

#endif // EMIT_DOC_HPP
