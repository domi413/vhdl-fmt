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

    // Compound assignment operators
    auto operator+=(const Doc &other) -> Doc &;  ///< Direct concatenation assignment
    auto operator&=(const Doc &other) -> Doc &;  ///< Space concatenation assignment
    auto operator/=(const Doc &other) -> Doc &;  ///< Softline assignment
    auto operator|=(const Doc &other) -> Doc &;  ///< Hardline assignment
    auto operator<<=(const Doc &other) -> Doc &; ///< Softline + indent rhs assignment

    [[nodiscard]]
    auto hardIndent(const Doc &other) const -> Doc; ///< Hardline + indent rhs

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

} // namespace emit

#endif // EMIT_DOC_HPP
