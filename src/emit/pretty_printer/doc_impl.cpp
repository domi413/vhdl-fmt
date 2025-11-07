#include "emit/pretty_printer/doc_impl.hpp"

#include "emit/pretty_printer/doc.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

namespace emit {

// Factory functions
auto makeEmpty() -> DocPtr
{
    return std::make_shared<DocImpl>(Empty{});
}

auto makeText(std::string_view text) -> DocPtr
{
    return std::make_shared<DocImpl>(Text{ std::string(text) });
}

auto makeLine() -> DocPtr
{
    return std::make_shared<DocImpl>(SoftLine{});
}

auto makeHardLine() -> DocPtr
{
    return std::make_shared<DocImpl>(HardLine{});
}

auto makeConcat(DocPtr left, DocPtr right) -> DocPtr
{
    return std::make_shared<DocImpl>(Concat{ .left = std::move(left), .right = std::move(right) });
}

auto makeNest(int indent, DocPtr doc) -> DocPtr
{
    return std::make_shared<DocImpl>(Nest{ .indent = indent, .doc = std::move(doc) });
}

auto makeUnion(DocPtr flat, DocPtr broken) -> DocPtr
{
    return std::make_shared<DocImpl>(Union{ .flat = std::move(flat), .broken = std::move(broken) });
}

// Helper for overload pattern
template<typename... Ts>
struct Overload : Ts... // NOLINT(fuchsia-multiple-inheritance)
{
    using Ts::operator()...;
};

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr
{
    if (!doc) {
        return doc;
    }

    return std::visit(Overload{
                        [](const Empty &) -> DocPtr { return makeEmpty(); },
                        [](const Text &node) -> DocPtr { return makeText(node.content); },
                        [](const SoftLine &) -> DocPtr {
                            // Line becomes a space when flattened
                            return makeText(" ");
                        },
                        [](const HardLine &) -> DocPtr {
                            // Hard line never becomes a space
                            return makeHardLine();
                        },
                        [](const Concat &node) -> DocPtr {
                            // Recursively flatten both sides
                            return makeConcat(flatten(node.left), flatten(node.right));
                        },
                        [](const Nest &node) -> DocPtr {
                            // Keep nest but flatten the inner doc
                            return makeNest(node.indent, flatten(node.doc));
                        },
                        [](const Union &node) -> DocPtr {
                            // Union already has flat version - just use it
                            return flatten(node.flat);
                        },
                      },
                      doc->value);
}

} // namespace emit
