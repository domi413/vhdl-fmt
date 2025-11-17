#include "emit/pretty_printer/doc_impl.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

using emit::DocPtr;
using emit::makeConcat;
using emit::makeEmpty;
using emit::makeHardLine;
using emit::makeHardLines;
using emit::makeText;

namespace {
// Helper lambda for counting all nodes in a Doc tree
constexpr auto NODE_COUNTER = [](int count, const auto & /*node*/) { return count + 1; };
} // namespace

TEST_CASE("Optimizer: Rule 1 (Identity) - a + empty", "[pretty_printer][optimizer]")
{
    constexpr std::string_view EXPECTED = "a";

    const DocPtr unoptimized = makeConcat(makeText("a"), makeEmpty());
    const DocPtr optimized = optimizeImpl(unoptimized);

    // Unoptimized: Concat(Text("a"), Empty) -> 3 nodes
    REQUIRE(foldImpl(unoptimized, 0, NODE_COUNTER) == 3);
    // Optimized: Text("a") -> 1 node
    REQUIRE(foldImpl(optimized, 0, NODE_COUNTER) == 1);

    // Check that optimized doc matches expected
    const auto *text_node = std::get_if<emit::Text>(&optimized->value);
    REQUIRE(text_node != nullptr);
    REQUIRE(text_node->content == EXPECTED);
}

TEST_CASE("Optimizer: Rule 1 (Identity) - empty + a", "[pretty_printer][optimizer]")
{
    constexpr std::string_view EXPECTED = "a";

    const DocPtr unoptimized = makeConcat(makeEmpty(), makeText("a"));
    const DocPtr optimized = optimizeImpl(unoptimized);

    // Unoptimized: Concat(Empty, Text("a")) -> 3 nodes
    REQUIRE(foldImpl(unoptimized, 0, NODE_COUNTER) == 3);
    // Optimized: Text("a") -> 1 node
    REQUIRE(foldImpl(optimized, 0, NODE_COUNTER) == 1);

    // Check that optimized doc matches expected
    const auto *text_node = std::get_if<emit::Text>(&optimized->value);
    REQUIRE(text_node != nullptr);
    REQUIRE(text_node->content == EXPECTED);
}

TEST_CASE("Optimizer: Rule 1 (Identity) - empty + empty", "[pretty_printer][optimizer]")
{
    const DocPtr unoptimized = makeConcat(makeEmpty(), makeEmpty());
    const DocPtr optimized = optimizeImpl(unoptimized);

    // Unoptimized: Concat(Empty, Empty) -> 3 nodes
    REQUIRE(foldImpl(unoptimized, 0, NODE_COUNTER) == 3);
    // Optimized: Empty -> 1 node
    REQUIRE(foldImpl(optimized, 0, NODE_COUNTER) == 1);

    // Check that optimized doc is an Empty node
    REQUIRE(std::holds_alternative<emit::Empty>(optimized->value));
}

TEST_CASE("Optimizer: Rule 2 (Text Merge)", "[pretty_printer][optimizer]")
{
    constexpr std::string_view EXPECTED = "ab";

    const DocPtr unoptimized = makeConcat(makeText("a"), makeText("b"));
    const DocPtr optimized = optimizeImpl(unoptimized);

    // Unoptimized: Concat(Text("a"), Text("b")) -> 3 nodes
    REQUIRE(foldImpl(unoptimized, 0, NODE_COUNTER) == 3);
    // Optimized: Text("ab") -> 1 node
    REQUIRE(foldImpl(optimized, 0, NODE_COUNTER) == 1);

    // Check that optimized doc matches expected
    const auto *text_node = std::get_if<emit::Text>(&optimized->value);
    REQUIRE(text_node != nullptr);
    REQUIRE(text_node->content == EXPECTED);
}

TEST_CASE("Optimizer: Rule 3 (HardLines Merge)", "[pretty_printer][optimizer]")
{
    constexpr unsigned EXPECTED = 6;

    // Manually build: (hardlines(2) + hardline()) + hardlines(3)
    const DocPtr unoptimized
      = makeConcat(makeConcat(makeHardLines(2), makeHardLine()), makeHardLines(3));
    const DocPtr optimized = optimizeImpl(unoptimized);

    // Unoptimized: Concat(Concat(HardLines(2), HardLine), HardLines(3)) -> 5 nodes
    REQUIRE(foldImpl(unoptimized, 0, NODE_COUNTER) == 5);
    // Optimized: HardLines(6) -> 1 node
    REQUIRE(foldImpl(optimized, 0, NODE_COUNTER) == 1);

    // Check that optimized doc matches expected
    const auto *lines_node = std::get_if<emit::HardLines>(&optimized->value);
    REQUIRE(lines_node != nullptr);
    REQUIRE(lines_node->count == EXPECTED);
}

TEST_CASE("Optimizer: Rule 3 (1 HardLines merges to HardLine)", "[pretty_printer][optimizer]")
{
    constexpr unsigned EXPECTED = 0;

    const DocPtr unoptimized = makeConcat(makeHardLines(0), makeHardLines(1));
    const DocPtr optimized = optimizeImpl(unoptimized);

    // Unoptimized: Concat(HardLines(0), HardLines(1)) -> 2 nodes
    REQUIRE(foldImpl(unoptimized, 0, NODE_COUNTER) == 3);
    // Optimized: HardLine -> 1 node
    REQUIRE(foldImpl(optimized, 0, NODE_COUNTER) == 1);

    // Check that optimized doc matches expected
    REQUIRE(std::get_if<emit::HardLine>(&optimized->value) != nullptr);
}
