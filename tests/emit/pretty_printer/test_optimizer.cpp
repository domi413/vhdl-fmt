#include "emit/pretty_printer/doc_impl.hpp"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <utility>
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

TEST_CASE("Smart Constructor: Rule 1 (Identity)", "[pretty_printer][smart_ctor]")
{
    // Case: a + empty -> a
    const DocPtr res1 = makeConcat(makeText("a"), makeEmpty());
    // Should immediately be Text("a"), count = 1
    REQUIRE(foldImpl(res1, 0, NODE_COUNTER) == 1);
    const auto *text1 = std::get_if<emit::Text>(&res1->value);
    REQUIRE(text1 != nullptr);
    REQUIRE(text1->content == "a");

    // Case: empty + a -> a
    const DocPtr res2 = makeConcat(makeEmpty(), makeText("a"));
    REQUIRE(foldImpl(res2, 0, NODE_COUNTER) == 1);

    // Case: empty + empty -> empty
    const DocPtr res3 = makeConcat(makeEmpty(), makeEmpty());
    REQUIRE(std::holds_alternative<emit::Empty>(res3->value));
}

TEST_CASE("Smart Constructor: Rule 2 (Text Merge)", "[pretty_printer][smart_ctor]")
{
    // makeConcat("a", "b") should returns Text("ab") directly
    const DocPtr res = makeConcat(makeText("a"), makeText("b"));

    REQUIRE(foldImpl(res, 0, NODE_COUNTER) == 1); // 1 node, not 3

    const auto *text = std::get_if<emit::Text>(&res->value);
    REQUIRE(text != nullptr);
    REQUIRE(text->content == "ab");
}

TEST_CASE("Smart Constructor: Rule 3 (HardLines Merge)", "[pretty_printer][smart_ctor]")
{
    // (hardlines(2) + hardline()) + hardlines(3)
    // Should collapse immediately into HardLines(6)
    const DocPtr res1 = makeConcat(makeConcat(makeHardLines(2), makeHardLine()), makeHardLines(3));

    REQUIRE(foldImpl(res1, 0, NODE_COUNTER) == 1); // 1 node, not 5

    const auto *lines1 = std::get_if<emit::HardLines>(&res1->value);
    REQUIRE(lines1 != nullptr);
    REQUIRE(lines1->count == 6);

    const DocPtr res2 = makeConcat(makeHardLines(1), makeHardLines(0));

    REQUIRE(foldImpl(res2, 0, NODE_COUNTER) == 1); // 1 node, not 2

    REQUIRE(std::get_if<emit::HardLine>(&res2->value) != nullptr);
}

TEST_CASE("Smart Constructor: Complex Text Chain (Fold)", "[pretty_printer][smart_ctor]")
{
    // Simulate building a long string from many small parts (e.g., a large identifier)
    constexpr auto PARTS = std::to_array<std::string_view>(
      { "This", " ", "is", " ", "a", " ", "complex", " ", "merge." });

    // Start with Empty (Rule 1 should eat this immediately)
    const DocPtr result = std::ranges::fold_left(PARTS, makeEmpty(), [](auto acc, const auto &str) {
        // Each makeConcat should trigger Rule 2 (Text Merge)
        // fusing the new part into the accumulator immediately.
        return makeConcat(std::move(acc), makeText(str));
    });

    // Verify the structure is fully flattened
    // Unoptimized: ((((((((Empty + "This") + " ") + "is") ... ) -> 19 nodes
    // Optimized: Text("This is a complex merge.") -> 1 node
    REQUIRE(foldImpl(result, 0, NODE_COUNTER) == 1);

    const auto *text = std::get_if<emit::Text>(&result->value);
    REQUIRE(text != nullptr);
    REQUIRE(text->content == "This is a complex merge.");
}

TEST_CASE("Smart Constructor: Interleaved Optimization", "[pretty_printer][smart_ctor]")
{
    // Test interaction between Rule 1 (Identity) and Rule 2 (Text Merge)
    // Structure: ("A" + (Empty + "B")) + ("C" + Empty)

    // -> "AB"
    const DocPtr lhs = makeConcat(makeText("A"), makeConcat(makeEmpty(), makeText("B")));
    const DocPtr rhs = makeConcat(makeText("C"), makeEmpty()); // -> "C"
    const DocPtr final_doc = makeConcat(lhs, rhs);             // -> "ABC"

    REQUIRE(foldImpl(final_doc, 0, NODE_COUNTER) == 1);

    const auto *text = std::get_if<emit::Text>(&final_doc->value);
    REQUIRE(text != nullptr);
    REQUIRE(text->content == "ABC");
}
