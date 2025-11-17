#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_impl.hpp"

#include <catch2/catch_test_macros.hpp>

using emit::Doc;
using emit::DocPtr;

namespace {
// Helper lambda for counting all nodes in a Doc tree
constexpr auto NODE_COUNTER = [](int count, const auto & /*node*/) { return count + 1; };
} // namespace

TEST_CASE("Optimizer: Rule 1 (Identity) - a + empty", "[pretty_printer][optimizer]")
{
    const Doc a = Doc::text("a");
    const Doc unoptimized_doc = a + Doc::empty();
    const Doc optimized_doc = unoptimized_doc.optimize();

    // Unoptimized: Concat(Text("a"), Empty) -> 3 nodes
    REQUIRE(unoptimized_doc.fold(0, NODE_COUNTER) == 3);
    // Optimized: Text("a") -> 1 node
    REQUIRE(optimized_doc.fold(0, NODE_COUNTER) == 1);
}

TEST_CASE("Optimizer: Rule 1 (Identity) - empty + a", "[pretty_printer][optimizer]")
{
    const Doc a = Doc::text("a");
    const Doc unoptimized_doc = Doc::empty() + a;
    const Doc optimized_doc = unoptimized_doc.optimize();

    // Unoptimized: Concat(Empty, Text("a")) -> 3 nodes
    REQUIRE(unoptimized_doc.fold(0, NODE_COUNTER) == 3);
    // Optimized: Text("a") -> 1 node
    REQUIRE(optimized_doc.fold(0, NODE_COUNTER) == 1);
}

TEST_CASE("Optimizer: Rule 1 (Identity) - empty + empty", "[pretty_printer][optimizer]")
{
    const Doc unoptimized_doc = Doc::empty() + Doc::empty();
    const Doc optimized_doc = unoptimized_doc.optimize();

    // Unoptimized: Concat(Empty, Empty) -> 3 nodes
    REQUIRE(unoptimized_doc.fold(0, NODE_COUNTER) == 3);
    // Optimized: Empty -> 1 node
    REQUIRE(optimized_doc.fold(0, NODE_COUNTER) == 1);
}

TEST_CASE("Optimizer: Rule 2 (Text Merge)", "[pretty_printer][optimizer]")
{
    const Doc unoptimized_doc = Doc::text("a") + Doc::text("b");
    const Doc optimized_doc = unoptimized_doc.optimize();

    // Unoptimized: Concat(Text("a"), Text("b")) -> 3 nodes
    REQUIRE(unoptimized_doc.fold(0, NODE_COUNTER) == 3);
    // Optimized: Text("ab") -> 1 node
    REQUIRE(optimized_doc.fold(0, NODE_COUNTER) == 1);
}

TEST_CASE("Optimizer: Rule 3 (HardLines Merge)", "[pretty_printer][optimizer]")
{
    const Doc unoptimized_doc = Doc::hardlines(2) + Doc::hardline() + Doc::hardlines(3);
    const Doc optimized_doc = unoptimized_doc.optimize();

    // Unoptimized: Concat(Concat(HardLines(2), HardLine), HardLines(3)) -> 5 nodes
    REQUIRE(unoptimized_doc.fold(0, NODE_COUNTER) == 5);
    // Optimized: HardLines(6) -> 1 node
    REQUIRE(optimized_doc.fold(0, NODE_COUNTER) == 1);
}
