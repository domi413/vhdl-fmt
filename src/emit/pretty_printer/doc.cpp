#include "emit/pretty_printer/doc.hpp"

#include "common/config.hpp"
#include "emit/pretty_printer/doc_impl.hpp"
#include "emit/pretty_printer/renderer.hpp"

#include <string>
#include <string_view>

namespace emit {

Doc::Doc() : impl_(makeEmpty()) {}

// Basic constructors
auto Doc::empty() -> Doc
{
    return Doc(makeEmpty());
}

auto Doc::text(std::string_view str) -> Doc
{
    return Doc(makeText(str));
}

auto Doc::line() -> Doc
{
    return Doc(makeLine());
}

auto Doc::hardline() -> Doc
{
    return Doc(makeHardLine());
}

auto Doc::alignText(std::string_view str) -> Doc // NEW
{
    return Doc(makeAlignPlaceholder(str));
}

// Combinators
auto Doc::operator+(const Doc &other) const -> Doc
{
    return Doc(makeConcat(impl_, other.impl_));
}

auto Doc::operator&(const Doc &other) const -> Doc
{
    return *this + Doc::text(" ") + other;
}

auto Doc::operator/(const Doc &other) const -> Doc
{
    return *this + line() + other;
}

auto Doc::operator|(const Doc &other) const -> Doc
{
    return *this + hardline() + other;
}

auto Doc::operator<<(const Doc &other) const -> Doc
{
    // *this + (line() + other).nest(DEFAULT_INDENT)
    auto nested = Doc(makeNest(makeConcat(line().impl_, other.impl_)));
    return *this + nested;
}

auto Doc::hardIndent(const Doc &other) const -> Doc
{
    // *this + (hardline() + other).nest(DEFAULT_INDENT)
    auto nested = Doc(makeNest(makeConcat(hardline().impl_, other.impl_)));
    return *this + nested;
}

// Compound assignment operators
auto Doc::operator+=(const Doc &other) -> Doc &
{
    *this = *this + other;
    return *this;
}

auto Doc::operator&=(const Doc &other) -> Doc &
{
    *this = *this & other;
    return *this;
}

auto Doc::operator/=(const Doc &other) -> Doc &
{
    *this = *this / other;
    return *this;
}

auto Doc::operator|=(const Doc &other) -> Doc &
{
    *this = *this | other;
    return *this;
}

auto Doc::operator<<=(const Doc &other) -> Doc &
{
    *this = *this << other;
    return *this;
}

auto Doc::bracket(const Doc &left, const Doc &inner, const Doc &right) -> Doc
{
    return (left << inner) / right;
}

auto Doc::group() const -> Doc
{
    return Doc(makeUnion(flatten(impl_), impl_));
}

// Rendering
auto Doc::render(const common::Config &config) const -> std::string
{
    Renderer renderer(config);
    return renderer.render(impl_);
}

auto Doc::resolveAlignment(const Doc &doc) -> Doc
{
    // === Pass 1: The "Fold" (using our new 'fold' method) ===
    int max_width = doc.fold(0, [](int current_max, const auto &node) {
        using T = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<T, AlignPlaceholder>) {
            return std::max(current_max, static_cast<int>(node.content.length()));
        }
        return current_max; // Pass accumulator through
    });

    // Handle the case where no aligned text was found
    if (max_width == 0) {
        return doc;
    }

    // === Pass 2: The "Transform" (unchanged) ===
    return doc.transform([&](const auto &node) -> DocPtr {
        using T = std::decay_t<decltype(node)>;

        if constexpr (std::is_same_v<T, AlignPlaceholder>) {
            int padding = max_width - static_cast<int>(node.content.length());
            return makeText(node.content + std::string(padding, ' '));
        } else {
            return std::make_shared<DocImpl>(node);
        }
    });
}

} // namespace emit
